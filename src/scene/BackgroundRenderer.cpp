// BackgroundRenderer class

# include "BackgroundRenderer.hpp"

BackgroundRenderer::BackgroundRenderer()
	: m_direct3D{ Direct3D::GetInstance() }
	, m_vertexBuffer{ nullptr }
	, m_vertexCount{ 0 }
{
	initialize();
}

void BackgroundRenderer::initialize()
{
	// 頂点情報の作成
	const std::vector<Vertex> vertices{ createVertices() };

	// 頂点数の計算
	m_vertexCount = static_cast<UINT>(vertices.size());

	// バッファの作成
	m_vertexBuffer = m_direct3D.createVertexBuffer(vertices);
}

std::vector<Vertex> BackgroundRenderer::createVertices() const
{
	const std::vector<Vertex> vertices{
		{ -1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f },	// 左下
		{ -1.0f,  1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f },	// 左上
		{  1.0f,  1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f },	// 右上

		{ -1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f },	// 左下
		{  1.0f,  1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f },	// 右上
		{  1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f },	// 右下
	};

	return vertices;
}

void BackgroundRenderer::draw() const
{
	// DirectXにTitleSceneのバッファを転送
	m_direct3D.setVertexBuffer(m_vertexBuffer);

	// 描画コマンド実行
	m_direct3D.draw(m_vertexCount);
}