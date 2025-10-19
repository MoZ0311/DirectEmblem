// BaseUnit class

# include "BaseUnit.hpp"

# include "../util/InputState.hpp"

using namespace Util;
using namespace FilePath;
using namespace Config::MapSettings;
using namespace Config::UnitSettings;

BaseUnit::BaseUnit()
	: m_direct3D{ Direct3D::GetInstance() }
	, m_unitIconTexture{ SlimeIconPath }
	, m_vertexCount{ 0 }
	, m_vertexBuffer{ nullptr }

	, m_unitType{ UnitType::None }
	, m_unitStatus{ 0, 0, 0, 0, 0 }
	, m_unitPosition{ 0, 0 }
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
	std::vector<Vertex> vertices{};

	const float startX{ -TileWidth * MapWidth / 2 };    // マップの開始x座標
	const float startY{ TileHeight * MapHeight / 2 };   // マップの開始y座標

	// 各辺の座標を計算
	const float left{ startX + m_unitPosition.x * TileWidth };
	const float right{ left + TileWidth };
	const float top{ startY - m_unitPosition.y * TileHeight };
	const float bottom{ top - TileHeight };

	// 色 (白(1.0, 1.0, 1.0, 1.0)に設定)
	const DirectX::XMFLOAT4 colorF{ 1.0f, 1.0f, 1.0f, 1.0f };

	// --- T1 (左下, 左上, 右上) ---

	// 頂点1:左下
	vertices.push_back({ { left, bottom, 0.0f }, colorF, { 0, 1 } });

	// 頂点1:左上
	vertices.push_back({ { left, top, 0.0f }, colorF, { 0, 0 } });

	// 頂点3:右上
	vertices.push_back({ { right, top, 0.0f }, colorF, { 1, 0 } });


	// --- T2 (左下, 右上, 右下) ---

	// 頂点4:左下
	vertices.push_back({ { left, bottom, 0.0f }, colorF, { 0, 1 } });

	// 頂点5:右上
	vertices.push_back({ { right, top, 0.0f }, colorF, { 1, 0 } });

	// 頂点6:右下
	vertices.push_back({ { right, bottom, 0.0f }, colorF, { 1, 1 } });

	return vertices;
}

void BaseUnit::update()
{
	GridPosition currentPosition{ m_unitPosition };
	if (InputState::KeyPressed(VK_UP) || InputState::KeyPressed(0x57))
	{
		--currentPosition.y;
	}
	if (InputState::KeyPressed(VK_DOWN) || InputState::KeyPressed(0x53))
	{
		++currentPosition.y;
	}
	if (InputState::KeyPressed(VK_LEFT) || InputState::KeyPressed(0x41))
	{
		--currentPosition.x;
	}
	if (InputState::KeyPressed(VK_RIGHT) || InputState::KeyPressed(0x44))
	{
		++currentPosition.x;
	}

	currentPosition.x = std::clamp(static_cast<int>(currentPosition.x), 0, MapWidth - 1);
	currentPosition.y = std::clamp(static_cast<int>(currentPosition.y), 0, MapHeight - 1);

	if (m_unitPosition.x != currentPosition.x ||
		m_unitPosition.y != currentPosition.y)
	{
		m_unitPosition = currentPosition;
		setUnitPosition(m_unitPosition);
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