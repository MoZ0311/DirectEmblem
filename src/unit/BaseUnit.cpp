// BaseUnit class

# include "BaseUnit.hpp"

# include "../util/InputState.hpp"

using namespace Util;
using namespace FilePath;
using namespace Config::MapSettings;
using namespace Config::UnitSettings;

BaseUnit::BaseUnit()
	: m_direct3D{ Direct3D::GetInstance() }
	, m_mapRenderer{ MapRenderer::GetInstance() }
	, m_unitIconTexture{ SlimeIconPath }
	, m_iconColor{ 1.0f, 1.0f, 1.0f, 1.0f }
	, m_vertexCount{ 0 }
	, m_vertexBuffer{ nullptr }

	, m_unitType{ UnitType::None }
	, m_unitStatus{ 0, 0, 0, 0, 0 }
	, m_unitPosition{ 0, 0 }
	, m_hasSelected{ false }
	, m_hasMoved{ false }
	, m_hasActed{ false }
{
	initialize();
}

void BaseUnit::initialize()
{
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
	const GridPosition mousePosition{ m_mapRenderer.getMouseGridPosition()};
	
	// ユニットの上にマウスがあるか
	const bool mouseOnUnit{ m_unitPosition.x == mousePosition.x && m_unitPosition.y == mousePosition.y };

	// 選択時の座標を格納する変数
	GridPosition prevPosition{};
	if (m_hasSelected)
	{
		if (InputState::KeyPressed(VK_RBUTTON))
		{
			// 右クリック(キャンセル)された時
			m_hasSelected = false;
			m_iconColor = { 1.0f, 1.0f, 1.0f, 1.0f };
			setUnitPosition(prevPosition);
		}
		else if (InputState::KeyPressed(VK_LBUTTON) && m_mapRenderer.getMouseOnMap())
		{
			// 目的地が設定された時
			setUnitPosition(mousePosition);
		}
	}
	else if (InputState::KeyPressed(VK_LBUTTON) && mouseOnUnit)
	{
		// 左クリック(選択)された時
		prevPosition = m_unitPosition;
		m_hasSelected = true;
		m_iconColor = { 1.0f, 1.0f, 0.3f, 1.0f };
		m_direct3D.updateVeretexBuffer(m_vertexBuffer, createVertices());
	}
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

	// 頂点バッファの更新処理
	m_direct3D.updateVeretexBuffer(m_vertexBuffer, createVertices());
}