// BaseUnit class

# include "BaseUnit.hpp"

# include "../unit/UnitManager.hpp"
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
	, m_accessibleTileGrid{}
	, m_movementPath{}
	, m_hasSelected{ false }
	, m_hasMoved{ false }
	, m_hasActed{ false }

	, m_gridMoveTimer{ GridMoveInterval }
{
	initialize();
}

void BaseUnit::initialize()
{
	// 侵入可能配列の初期化
	m_accessibleTileGrid.assign(MapHeight, std::vector<int>(MapWidth, -1));

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

void BaseUnit::update()
{
	// マウスのグリッド座標を取得
	const GridPosition mousePosition{ m_fieldMap.getMouseGridPosition()};
	
	// ユニットの上にマウスがあるか
	const bool mouseOnUnit{ m_unitPosition.x == mousePosition.x && m_unitPosition.y == mousePosition.y };

	if (m_hasSelected)
	{
		// 選択中は、アイコンを黄色に
		m_iconColor = { 1.0f, 1.0f, 0.3f, 1.0f };

		if (InputState::KeyPressed(VK_RBUTTON))
		{
			// 右クリック(キャンセル)された時
			UnitManager::GetInstance().isUnitMoving = false;
			m_hasSelected = false;
			m_hasMoved = false;
			m_unitPosition = m_prevPosition;
		}
		else if (InputState::KeyPressed(VK_LBUTTON) && 
				 m_fieldMap.getMouseOnMap() &&
				 m_accessibleTileGrid[mousePosition.y][mousePosition.x] > -1)
		{
			// 有効な移動先が左クリック(選択)された時
			createMovementPath(mousePosition);
		}
	}
	else
	{
		// 非選択中は、アイコンを白色に
		m_iconColor = { 1.0f, 1.0f, 1.0f, 1.0f };

		if (InputState::KeyPressed(VK_LBUTTON) && mouseOnUnit &&
			!UnitManager::GetInstance().isUnitMoving)
		{
			// 左クリック(選択)された時
			UnitManager::GetInstance().isUnitMoving = true;
			m_prevPosition = m_unitPosition;
			m_hasSelected = true;
			
			// 移動範囲の算出
			calculateMovementRange();

			// 移動範囲を渡す
			m_fieldMap.setAccessibleTileGrid(m_accessibleTileGrid);
		}
	}

	// 移動処理
	gridMove();

	// バッファの更新
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

void BaseUnit::gridMove()
{
	// 経路配列は空であるか
	if (m_movementPath.empty())
	{
		/*
		if (m_hasMoved)
		{
			m_hasSelected = false;
			UnitManager::GetInstance().isUnitMoving = false;
		}
		m_hasMoved = true;
		*/

		return;
	}
	else
	{
		// 配列を辿りながら消去
		m_unitPosition = m_movementPath.top();
		m_movementPath.pop();
	}

	/*
	--m_gridMoveTimer;

	// タイマー0で処理が進む
	if (m_gridMoveTimer < 0)
	{
		

		// タイマーリセット
		m_gridMoveTimer = GridMoveInterval;
	}
	*/
}

void BaseUnit::calculateMovementRange()
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
			const TileType currentTileType{ mapData[y][x] };

			// 侵入コストを対応表から取得
			accessCostGrid[y][x] = TileAccessCost.at(currentTileType);
		}
	}

	// 探索の為のキューを作成
	std::queue<std::pair<GridPosition, int>> searchQueue{};

	// 現在地に初期移動力を設定
	const int defaultMobility{ m_unitParameter.mobility };
	m_accessibleTileGrid[m_unitPosition.y][m_unitPosition.x] = defaultMobility;

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
		for (const auto& direction : GridOffset)
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
				if (nextMobility > m_accessibleTileGrid[nextPosition.y][nextPosition.x])
				{
					// m_distanceGridを更新
					m_accessibleTileGrid[nextPosition.y][nextPosition.x] = nextMobility;

					// 次の探索先をキューに追加
					searchQueue.push({ nextPosition, nextMobility });
				}
			}
		}
	}
}

void BaseUnit::createMovementPath(const Config::MapSettings::GridPosition& targetPosition)
{
	// 空の経路を作成
	std::stack<GridPosition> emptyPath{};

	// 空の経路とスワップ
	m_movementPath.swap(emptyPath);

	// 目的地を設定
	m_movementPath.push(targetPosition);

	// 目的地から現在地まで探索
	while (!(m_movementPath.top().x == m_unitPosition.x && m_movementPath.top().y == m_unitPosition.y))
	{
		// 範囲for文で、隣接4マスの始点からの距離を評価
		for (const auto& direction : GridOffset)
		{
			// 次の探索目標を設定
			const GridPosition nextPosition{
				m_movementPath.top().x + direction.x,
				m_movementPath.top().y + direction.y
			};

			// マップの境界チェック
			if (nextPosition.x < 0 || nextPosition.x >= MapWidth ||
				nextPosition.y < 0 || nextPosition.y >= MapHeight)
			{
				continue;
			}

			// 隣接するマスの中で、侵入可能かつ最も始点に近いものを格納
			const int nextAccessTile{ m_accessibleTileGrid[nextPosition.y][nextPosition.x] };
			const int currentAccessTile{ m_accessibleTileGrid[m_movementPath.top().y][m_movementPath.top().x] };

			if (nextAccessTile > -1 &&
				nextAccessTile > currentAccessTile)
			{
				m_movementPath.push(nextPosition);
			}
		}
	}
}

std::vector<std::vector<int>> BaseUnit::getAccessibleTileGrid() const
{
	return m_accessibleTileGrid;
}