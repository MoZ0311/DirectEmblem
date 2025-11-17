// UnitBase class

# include "UnitBase.hpp"

# include "../unit/UnitManager.hpp"
# include "../scene/SceneManager.hpp"
# include "../dx11/Direct3D.hpp"
# include "../map/FieldMap.hpp"
# include "../util/InputState.hpp"
# include "../util/PathFinder.hpp"

using namespace Util;
using namespace FilePath;
using namespace Config::MapSettings;
using namespace Config::UnitSettings;
using namespace Config::UISettings;

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
	, m_unitState{ UnitState::None }

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

	// 頂点バッファの作成
	m_vertexBuffer = m_direct3D.createVertexBuffer(vertices);
}

std::vector<Vertex> UnitBase::createVertices() const
{
	// 形状は常に TileWidth x TileHeight の四角形（ローカル座標）
	// 中心を (0, 0) とし、draw()でワールド行列により位置を設定する。
	const float halfWidth{ TileWidth / 2.0f };
	const float halfHeight{ TileHeight / 2.0f };

	const DirectX::XMFLOAT4 color{ 1.0f, 1.0f, 1.0f, 1.0f };    // 色はデフォルト

	// uv座標定義
	const DirectX::XMFLOAT2 uvTopLeft{ 0, 0 };		// 左上
	const DirectX::XMFLOAT2 uvBottomLeft{ 0, 1 };	// 左下
	const DirectX::XMFLOAT2 uvTopRight{ 1, 0 };		// 右上
	const DirectX::XMFLOAT2 uvBottomRight{ 1, 1 };	// 右下

	const std::vector<Vertex> vertices{
		// 頂点1:左下
		{
			{ -halfWidth, -halfHeight, 0.0f }, color, uvBottomLeft
		},
		// 頂点2:左上
		{
			{ -halfWidth, halfHeight, 0.0f }, color, uvTopLeft
		},
		// 頂点3:右上
		{
			{ halfWidth, halfHeight, 0.0f }, color, uvTopRight
		},
		// 頂点4:左下
		{
			{ -halfWidth, -halfHeight, 0.0f }, color, uvBottomLeft
		},
		// 頂点5:右上
		{
			{ halfWidth, halfHeight, 0.0f }, color, uvTopRight
		},
		// 頂点6:右下
		{
			{ halfWidth, -halfHeight, 0.0f }, color, uvBottomRight
		}
	};

	return vertices;
}

void UnitBase::update()
{
	// マウスのグリッド座標を取得
	const GridPosition mousePosition{ m_fieldMap.getMouseGridPosition()};
	
	// ユニットの上にマウスがあるか
	const bool mouseOnUnit{ m_unitPosition == mousePosition };

	switch (m_unitState)
	{
	case UnitState::None:		// 選択前

		// 左クリック(選択)された時
		if (InputState::KeyDown(VK_LBUTTON) && mouseOnUnit &&
			!UnitManager::GetInstance().isUnitMoving)
		{
			UnitManager::GetInstance().isUnitMoving = true;
			m_prevPosition = m_unitPosition;

			// 選択中は、アイコンを黄色に
			m_iconColor = { 1.0f, 1.0f, 0.3f, 1.0f };

			// 現在地からの距離の算出
			m_distanceGrid = PathFinder::CalculateDistanceGrid(m_unitPosition, m_unitParameter.mobility);

			// 距離と移動力を渡す
			m_fieldMap.setAccessibleTileGrid(m_distanceGrid, m_unitParameter.mobility);

			// 選択後のステートに移動
			m_unitState = UnitState::StandBy;
		}
		break;

	case UnitState::StandBy:	// 選択後

		// 右クリック(キャンセル)された時
		if (InputState::KeyDown(VK_RBUTTON))
		{
			UnitManager::GetInstance().isUnitMoving = false;
			m_unitPosition = m_prevPosition;

			// 非選択中は、アイコンを白色に
			m_iconColor = { 1.0f, 1.0f, 1.0f, 1.0f };

			// 選択前のステートに戻る
			m_unitState = UnitState::None;
		}
		// 有効な移動先が左クリック(選択)された時
		else if (InputState::KeyDown(VK_LBUTTON) &&
			m_fieldMap.getMouseOnMap() &&
			m_fieldMap.getAccessibleTileGrid()[mousePosition.y][mousePosition.x])
		{
			// 移動経路を作成
			m_movementPath = PathFinder::CreateMovementPath(m_distanceGrid, mousePosition);

			// 移動中ステートに移動
			m_unitState = UnitState::Moving;
		}
		break;

	case UnitState::Moving:		// 移動中

		// 経路は空であるか
		if (m_movementPath.empty())
		{
			// 経路が空であれば、コマンド選択ステートに移動
			m_unitState = UnitState::Acting;
		}
		else
		{
			// 経路が空でなければ、移動処理
			gridMove();
		}
		
		break;
	case UnitState::Acting:		// 行動中

		// 右クリック(キャンセル)された時
		if (InputState::KeyDown(VK_RBUTTON))
		{
			UnitManager::GetInstance().isUnitMoving = false;
			m_unitPosition = m_prevPosition;

			// 非選択中は、アイコンを白色に
			m_iconColor = { 1.0f, 1.0f, 1.0f, 1.0f };

			// 選択前のステートに戻る
			m_unitState = UnitState::None;
		}
		break;

	case UnitState::Waiting:	// 行動終了
		break;
	default:
		break;
	}
}

void UnitBase::draw() const
{
	// 画面上の座標を設定
	DirectX::XMFLOAT2 screenUnitPosition{ m_fieldMap.gridToScreen(m_unitPosition) };

	// 計算した位置をワールド行列に変換
	DirectX::XMMATRIX worldMatrix{ DirectX::XMMatrixTranslation(screenUnitPosition.x, screenUnitPosition.y, 0.0f) };

	// 定数バッファの設定
	Util::ObjectConstants constants{};
	DirectX::XMStoreFloat4x4(&constants.worldMatrix, DirectX::XMMatrixTranspose(worldMatrix));
	
	// ビュー行列とプロジェクション行列の設定
	DirectX::XMStoreFloat4x4(&constants.viewMatrix, DirectX::XMMatrixIdentity());
	DirectX::XMStoreFloat4x4(&constants.projectionMatrix, DirectX::XMMatrixIdentity());

	// 色の設定
	constants.color = m_iconColor;

	// 定数バッファを更新
	m_direct3D.updateConstantBuffer(constants);

	// テクスチャのセット
	m_direct3D.setTexture(m_unitIconTexture.getShaderResourceView());

	// DirectXにバッファを転送
	m_direct3D.setVertexBuffer(m_vertexBuffer);

	// 描画コマンド実行
	m_direct3D.draw(m_vertexCount);
}

void UnitBase::onFinishActed(const Config::UISettings::Command& selectedCommand)
{
	switch (selectedCommand)
	{
	case Command::Attack:
	case Command::Skill:
	case Command::Item:

	case Command::Wait:	// 待機

		UnitManager::GetInstance().isUnitMoving = false;

		// アイコンを灰色に
		m_iconColor = { 0.6f, 0.6f, 0.6f, 1.0f };
	default:
		break;
	}
}

void UnitBase::setPosition(const Config::MapSettings::GridPosition& targetPosition)
{
	// 指定座標に移動
	m_unitPosition = targetPosition;
}

void UnitBase::gridMove()
{
	const float deltaTime{ SceneManager::GetInstance().getDeltaTime() };
	m_gridMoveTimer -= deltaTime;

	// タイマーが0より小さくなったとき、処理を実行
	if (m_gridMoveTimer < 0)
	{
		// 配列を辿りながら消去
		const GridPosition nextPosition{ m_movementPath.front() };
		m_movementPath.pop_front();

		setPosition(nextPosition);

		// タイマーリセット
		m_gridMoveTimer = GridMoveInterval;
	}
}

Config::MapSettings::GridPosition UnitBase::getUnitPosition() const
{
	return m_unitPosition;
}

UnitState UnitBase::getUnitState() const
{
	return m_unitState;
}