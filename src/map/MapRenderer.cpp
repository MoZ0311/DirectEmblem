// MapRenderer class

# include "MapRenderer.hpp"

using namespace Config::MapSettings;

MapRenderer::MapRenderer()
	: m_mapData{ TileType::Grass }
	, m_direct3D{ Direct3D::GetInstance() }
	, m_vertexBuffer{ nullptr }
	, m_vertexCount{ 0 }
{
	initialize();
}

void MapRenderer::initialize()
{
	// 頂点情報の作成
	std::vector<Vertex> vertices{ createVertices() };

	// 頂点数の計算
	m_vertexCount = static_cast<UINT>(vertices.size());

	// バッファの作成
	m_vertexBuffer = m_direct3D.createVertexBuffer(vertices);
}

std::vector<Vertex> MapRenderer::createVertices() const
{
	// 画面中央に配置する一つの正方形の頂点データ
	const std::vector<Vertex> vertices{
		// 頂点データ: { x, y, z, r, g, b, a } 
		{  0.0f, -0.9f,  0.0f, 1.0f, 0.0f, 1.0f, 1.0f },	// 下
		{ -0.9f,  0.9f,  0.0f, 1.0f, 1.0f, 0.0f, 1.0f },	// 左上
		{  0.9f,  0.9f,  0.0f, 0.0f, 1.0f, 1.0f, 1.0f },	// 右上
	};

	return vertices;
}

void MapRenderer::update()
{

}

void MapRenderer::draw() const
{
	// DirectXにTitleSceneのバッファを転送
	m_direct3D.setVertexBuffer(m_vertexBuffer);

	// 描画コマンド実行
	m_direct3D.draw(m_vertexCount);
}