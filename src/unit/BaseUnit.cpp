// BaseUnit class

# include "BaseUnit.hpp"

# include "../util/InputState.hpp"

using namespace Util;
using namespace FilePath;
using namespace Config::MapSettings;
using namespace Config::UnitSettings;

BaseUnit::BaseUnit()
	: m_direct3D{ Direct3D::GetInstance() }
	, m_fieldMap{ FieldMap::GetInstance() }
	, m_unitIconTexture{ SlimeIconPath }
	, m_iconColor{ 1.0f, 1.0f, 1.0f, 1.0f }
	, m_vertexCount{ 0 }
	, m_vertexBuffer{ nullptr }

	, m_unitType{ UnitType::None }
	, m_unitParameter{ 0, 0, 0, 0, 0 }
	, m_unitPosition{ 0, 0 }
	, m_prevPosition{ m_unitPosition }
	, m_distanceGrid{}
	, m_hasSelected{ false }
	, m_hasMoved{ false }
	, m_hasActed{ false }
{
	initialize();
}

void BaseUnit::initialize()
{
	// 距離配列の初期化
	m_distanceGrid.assign(MapHeight, std::vector<int>(MapWidth, -1));

	// 頂点情報の作成
	const std::vector<Vertex> vertices{ createVertices() };

	// 頂点数の計算
	m_vertexCount = static_cast<UINT>(vertices.size());

	// バッファの作成
	m_vertexBuffer = m_direct3D.createVertexBuffer(vertices);
}

std::vector<Vertex> BaseUnit::createVertices() const
{
	// 各辺の座標を計算
	const float left{ MapStartX + m_unitPosition.x * TileWidth };
	const float right{ left + TileWidth };
	const float top{ MapStartY - m_unitPosition.y * TileHeight };
	const float bottom{ top - TileHeight };

	// uv座標定義
	const DirectX::XMFLOAT2 uvTopLeft{ 0, 0 };		// 左上
	const DirectX::XMFLOAT2 uvBottomLeft{ 0, 1 };	// 左下
	const DirectX::XMFLOAT2 uvTopRight{ 1, 0 };		// 右上
	const DirectX::XMFLOAT2 uvBottomRight{ 1, 1 };	// 右下

	const std::vector<Vertex> vertices{
		// 頂点1:左下
		{
			{ left, bottom, 0.0f }, m_iconColor, uvBottomLeft
		},
		// 頂点2:左上
		{
			{ left, top, 0.0f }, m_iconColor, uvTopLeft
		},
		// 頂点3:右上
		{
			{ right, top, 0.0f }, m_iconColor, uvTopRight
		},
		// 頂点4:左下
		{
			{ left, bottom, 0.0f }, m_iconColor, uvBottomLeft
		},
		// 頂点5:右上
		{
			{ right, top, 0.0f }, m_iconColor, uvTopRight
		},
		// 頂点6:右下
		{
			{ right, bottom, 0.0f }, m_iconColor, uvBottomRight
		}
	};
	return vertices;
}

void BaseUnit::calculateDistance()
{
	// マップデータの取得
	const std::vector<std::vector<int>> mapData{ m_fieldMap.getMapData() };

	// 二重ループで移動力の配列を作成
	std::vector<std::vector<int>> accessCostGrid{};
	accessCostGrid.assign(MapHeight, std::vector<int>(MapWidth, -1));

	for (int y{ 0 }; y < MapHeight; ++y)
	{
		for (int x{ 0 }; x < MapWidth; ++x)
		{
			// mapDataのint型をTileTypeに変換
			const TileType currentTileType{ mapData[y][x]};

			// 侵入コストを対応表から取得
			accessCostGrid[y][x] = TileAccessCost.at(currentTileType);
		}
	}

	// 距離配列を計算
	// グリッド座標における上下左右を定義
	const GridPosition offset[4]{
		{ 0, -1 },	// グリッドの上方向
		{ -1, 0 },	// グリッドの左方向
		{ 1, 0 },	// グリッドの右方向
		{ 0, 1 },	// グリッドの下方向
	};

	// 探索の為のキューを作成
	std::queue<std::pair<GridPosition, int>> searchQueue{};

	// 現在地に初期移動力を設定
	const int defaultMobility{ m_unitParameter.mobility };
	m_distanceGrid[m_unitPosition.y][m_unitPosition.x] = defaultMobility;
	
	// 探索キューに初期位置と初期移動力を渡す
	searchQueue.push({ m_unitPosition, defaultMobility });

	// キューが空になる(これ以上探索できない)まで繰り返す
	while (!searchQueue.empty())
	{
		// キューの先頭を取り出す
		const auto current{ searchQueue.front() };
		searchQueue.pop();

		// 現在地を定義
		const GridPosition currentPositon{ current.first };

		// 現在の移動力を定義
		const int currentMobility{ current.second };

		// offsetで定義した四方向に範囲for文でアクセス
		for (const auto& direction : offset)
		{
			// アクセス先のグリッド座標を定義
			const GridPosition nextPosition{
				currentPositon.x + direction.x,
				currentPositon.y + direction.y
			};

			// マップの境界チェック
			if (nextPosition.x < 0 || nextPosition.x >= MapWidth ||
				nextPosition.y < 0 || nextPosition.y >= MapHeight)
			{
				// 範囲外アクセス時に、continue
				continue;
			}

			// mapDataのint型をTileTypeに変換
			const TileType nextTileType{ static_cast<TileType>(mapData[nextPosition.y][nextPosition.x]) };

			// 侵入コストを対応表から取得
			const int accessCost{ TileAccessCost.at(nextTileType) };

			// 残移動力が侵入コスト以上なら移動可能
			if (currentMobility >= accessCost)
			{
				// 現在の移動力から、侵入コストを引いて次の残り移動力を算出
				const int nextMobility{ currentMobility - accessCost };

				// m_distanceGridの更新チェック (既に記録されている値より大きい、つまりより遠くまで行けるか)
				if (nextMobility > m_distanceGrid[nextPosition.y][nextPosition.x])
				{
					// m_distanceGridを更新
					m_distanceGrid[nextPosition.y][nextPosition.x] = nextMobility;

					// 次の探索先をキューに追加
					searchQueue.push({ nextPosition, nextMobility });
				}
			}
		}
	}
}

void BaseUnit::update()
{
	// マウスのグリッド座標を取得
	const GridPosition mousePosition{ m_fieldMap.getMouseGridPosition()};
	
	// ユニットの上にマウスがあるか
	const bool mouseOnUnit{ m_unitPosition.x == mousePosition.x && m_unitPosition.y == mousePosition.y };

	// 選択時の座標を格納する変数
	if (m_hasSelected)
	{
		// 選択中は、アイコンを黄色に
		m_iconColor = { 1.0f, 1.0f, 0.3f, 1.0f };

		if (InputState::KeyPressed(VK_RBUTTON))
		{
			// 右クリック(キャンセル)された時
			m_hasSelected = false;
			m_hasMoved = false;
			setUnitPosition(m_prevPosition);
		}
		else if (InputState::KeyPressed(VK_LBUTTON) && 
				 m_fieldMap.getMouseOnMap() &&
				 m_distanceGrid[mousePosition.y][mousePosition.x] > -1)
		{
			// 有効な移動先が左クリック(選択)された時
			setUnitPosition(mousePosition);
			m_hasMoved = true;
		}
	}
	else
	{
		// 非選択中は、アイコンを白色に
		m_iconColor = { 1.0f, 1.0f, 1.0f, 1.0f };

		if (InputState::KeyPressed(VK_LBUTTON) && mouseOnUnit)
		{
			// 左クリック(選択)された時
			m_prevPosition = m_unitPosition;
			m_hasSelected = true;
			
			// 移動範囲の算出
			calculateDistance();
		}
	}

	m_direct3D.updateVeretexBuffer(m_vertexBuffer, createVertices());
}

void BaseUnit::draw() const
{
	// 背景テクスチャのセット
	m_direct3D.setTexture(m_unitIconTexture.getShaderResourceView());

	// DirectXにTitleSceneのバッファを転送
	m_direct3D.setVertexBuffer(m_vertexBuffer);

	// 描画コマンド実行
	m_direct3D.draw(m_vertexCount);
}

void BaseUnit::setUnitPosition(const GridPosition& targetPoint)
{
	// 位置変更
	m_unitPosition = targetPoint;
}