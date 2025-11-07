// UnitBase class

# include "UnitBase.hpp"

# include "../unit/UnitManager.hpp"
# include "../scene/SceneManager.hpp"
# include "../dx11/Direct3D.hpp"
# include "../map/FieldMap.hpp"
# include "../util/InputState.hpp"

using namespace Util;
using namespace FilePath;
using namespace Config::MapSettings;
using namespace Config::UnitSettings;

UnitBase::UnitBase()
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
	, m_movementPath{}
	, m_isSelecting{ false }
	, m_isMoving{ false }
	, m_isActing{ false }

	, m_gridMoveTimer{ GridMoveInterval }
{
	initialize();
}

void UnitBase::initialize()
{
	// 頂点情報の作成
	const std::vector<Vertex> vertices{ createVertices() };

	// 頂点数の計算
	m_vertexCount = static_cast<UINT>(vertices.size());

	// バッファの作成
	m_vertexBuffer = m_direct3D.createVertexBuffer(vertices);
}

std::vector<Vertex> UnitBase::createVertices() const
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

void UnitBase::update()
{
	// マウスのグリッド座標を取得
	const GridPosition mousePosition{ m_fieldMap.getMouseGridPosition()};
	
	// ユニットの上にマウスがあるか
	const bool mouseOnUnit{ m_unitPosition.x == mousePosition.x && m_unitPosition.y == mousePosition.y };

	if (m_isSelecting)
	{
		if (InputState::KeyPressed(VK_RBUTTON))
		{
			// 右クリック(キャンセル)された時
			UnitManager::GetInstance().isUnitMoving = false;
			m_isSelecting = false;
			m_isMoving = false;
			m_isActing = false;
			m_unitPosition = m_prevPosition;

			// 非選択中は、アイコンを白色に
			m_iconColor = { 1.0f, 1.0f, 1.0f, 1.0f };

			// バッファの更新
			m_direct3D.updateVeretexBuffer(m_vertexBuffer, createVertices());
		}
		else if (InputState::KeyPressed(VK_LBUTTON) && 
				 m_fieldMap.getMouseOnMap() &&
				 m_fieldMap.getAccessibleTileGrid()[mousePosition.y][mousePosition.x])
		{
			// 有効な移動先が左クリック(選択)された時
			m_isMoving = true;
			createMovementPath(mousePosition);
		}
	}
	else
	{
		if (InputState::KeyPressed(VK_LBUTTON) && mouseOnUnit &&
			!UnitManager::GetInstance().isUnitMoving)
		{
			// 左クリック(選択)された時
			UnitManager::GetInstance().isUnitMoving = true;
			m_prevPosition = m_unitPosition;
			m_isSelecting = true;

			// 選択中は、アイコンを黄色に
			m_iconColor = { 1.0f, 1.0f, 0.3f, 1.0f };
			
			// 移動範囲の算出
			calculateMovementRange();

			// 移動範囲と移動力を渡す
			m_fieldMap.setAccessibleTileGrid(m_distanceGrid, m_unitParameter.mobility);

			// バッファの更新
			m_direct3D.updateVeretexBuffer(m_vertexBuffer, createVertices());
		}
	}

	// 移動処理
	gridMove();
}

void UnitBase::draw() const
{
	// 背景テクスチャのセット
	m_direct3D.setTexture(m_unitIconTexture.getShaderResourceView());

	// DirectXにTitleSceneのバッファを転送
	m_direct3D.setVertexBuffer(m_vertexBuffer);

	// 描画コマンド実行
	m_direct3D.draw(m_vertexCount);
}

void UnitBase::setPosition(const Config::MapSettings::GridPosition& targetPosition)
{
	// 指定座標に移動
	m_unitPosition = targetPosition;

	// バッファの更新
	m_direct3D.updateVeretexBuffer(m_vertexBuffer, createVertices());
}

void UnitBase::gridMove()
{
	const float deltaTime{ SceneManager::GetInstance().getDeltaTime() };
	m_gridMoveTimer -= deltaTime;

	// タイマーが0より小さくなったとき、処理を実行
	if (m_gridMoveTimer < 0)
	{
		// 経路配列は空であるか
		if (m_movementPath.empty())
		{
			if (m_isMoving)
			{
				m_isMoving = false;
				UnitManager::GetInstance().isUnitMoving = false;
				m_isActing = true;
			}
		}
		else
		{
			// 配列を辿りながら消去
			setPosition(m_movementPath.front());
			m_movementPath.pop_front();
		}

		// タイマーリセット
		m_gridMoveTimer = GridMoveInterval;
	}
}

void UnitBase::calculateMovementRange()
{
	// 距離配列を巨大な値で初期化
	m_distanceGrid.assign(MapHeight, std::vector<int>(MapWidth, INT_MAX));

	// 移動力と座標のペアを定義
	using Pair = std::pair<int, GridPosition>;

	// 探索の為のキューを作成
	std::priority_queue<Pair, std::vector<Pair>, std::greater<Pair>> searchQueue{};

	// 現在地の距離を0にする
	m_distanceGrid[m_unitPosition.y][m_unitPosition.x] = 0;

	// 探索キューに初期位置を渡す
	searchQueue.emplace(0, m_unitPosition);

	// キューが空になる(これ以上探索できない)まで繰り返す
	while (!searchQueue.empty())
	{
		// キューの先頭を取り出す
		const Pair searchPair{ searchQueue.top() };
		searchQueue.pop();

		const int searchDistance{ searchPair.first };
		const GridPosition searchPosition{ searchPair.second };

		// 最短でなければ、処理しない
		const int recordedDistance{ m_distanceGrid[searchPosition.y][searchPosition.x] };
		if (recordedDistance < searchDistance)
		{
			continue;
		}

		// offsetで定義した四方向に範囲for文でアクセス
		for (const auto& direction : GridOffset)
		{
			// アクセス先のグリッド座標を定義
			const GridPosition nextPosition{
				searchPosition.x + direction.x,
				searchPosition.y + direction.y
			};

			// マップの境界チェック
			if (nextPosition.x < 0 || nextPosition.x >= MapWidth ||
				nextPosition.y < 0 || nextPosition.y >= MapHeight)
			{
				// 範囲外アクセス時に、continue
				continue;
			}

			// ユニットが存在するタイルを取得
			const std::vector<std::vector<bool>> unitStandingGrid{ UnitManager::GetInstance().getUnitStandingGrid() };

			// ユニットがいるマスは、侵入不可としてスキップ
			if (unitStandingGrid[nextPosition.y][nextPosition.x]) 
			{
				// ユニットがいるマスは経路として使わない
				continue;
			}

			// 現在の距離を算出
			const int currentDistance{ m_distanceGrid[searchPosition.y][searchPosition.x] };

			// マップデータの取得
			const std::vector<std::vector<int>> mapData{ m_fieldMap.getMapData() };

			// mapDataのint型をTileTypeに変換
			const TileType nextTileType{ static_cast<TileType>(mapData[nextPosition.y][nextPosition.x]) };

			// 侵入コストを対応表から取得
			const int accessCost{ TileAccessCost.at(nextTileType) };

			// 新しい距離を計算
			const int newDistance{ currentDistance + accessCost };

			if (newDistance < m_distanceGrid[nextPosition.y][nextPosition.x])
			{
				// m_distanceGridを更新
				m_distanceGrid[nextPosition.y][nextPosition.x] = newDistance;

				// 次の探索先をキューに追加
				searchQueue.emplace(newDistance, nextPosition);
			}			
		}
	}	
}

void UnitBase::createMovementPath(const GridPosition& targetPosition)
{
	if (targetPosition == m_unitPosition)
	{
		// 現在地が指定された時、移動しない
		return;
	}
	
	// 仮の移動経路を作成
	std::deque<GridPosition> path{};

	// 目的地から探索を始める
	GridPosition currentPosition{ targetPosition };

	// 目的地から現在地まで探索
	while (currentPosition != m_unitPosition)
	{
		// 仮のパスに現在地を格納
		path.push_front(currentPosition);

		// 現在地までの距離(移動コストを考慮)を取得
		const int currentDistance{ m_distanceGrid[currentPosition.y][currentPosition.x] };

		// 範囲for文で、隣接4マスの始点からの距離を評価
		for (const auto& direction : GridOffset)
		{
			// 次の探索目標を設定
			const GridPosition nextPosition{
				currentPosition.x + direction.x,
				currentPosition.y + direction.y
			};

			// マップの境界チェック
			if (nextPosition.x < 0 || nextPosition.x >= MapWidth ||
				nextPosition.y < 0 || nextPosition.y >= MapHeight)
			{
				continue;
			}

			// 次のマスへの距離を算出
			const int nextDistance{ m_distanceGrid[nextPosition.y][nextPosition.x] };

			// 現在地より遠くへは行かない
			if (currentDistance < nextDistance)
			{
				continue;
			}

			// マップデータの取得
			const std::vector<std::vector<int>> mapData{ m_fieldMap.getMapData() };

			// mapDataのint型をTileTypeに変換
			const TileType nextTileType{ static_cast<TileType>(mapData[currentPosition.y][currentPosition.x]) };

			// 侵入コストを対応表から取得
			const int accessCost{ TileAccessCost.at(nextTileType) };

			// 新しい距離を計算
			const int newDistance{ nextDistance + accessCost };

			if (newDistance == currentDistance)
			{
				currentPosition = nextPosition;
				break;
			}
		}
	}

	// 最後にスタート地点を追加
	path.push_front(m_unitPosition);

	// 完成した経路をメンバ変数に渡す
	m_movementPath = path;
}

Config::MapSettings::GridPosition UnitBase::getUnitPosition() const
{
	return m_unitPosition;
}

bool UnitBase::getIsActing() const
{
	return m_isActing;
}