// BackgroundRenderer class

# include "BackgroundRenderer.hpp"

BackgroundRenderer::BackgroundRenderer()
	: m_direct3D{ Direct3D::GetInstance() }
	, m_vertexCount{ 0 }
	, m_vertexBuffer{ nullptr }
{
	initialize();
}

void BackgroundRenderer::initialize()
{
	// 背景テクスチャのロード
    Texture texture{ Config::BackgroundPath };

    // 背景テクスチャのセット
    m_direct3D.setTexture(texture.getShaderResourceView());

	// 頂点情報の作成
	const std::vector<Vertex> vertices{ createVertices() };

	// 頂点数の計算
	m_vertexCount = static_cast<UINT>(vertices.size());

	// バッファの作成
	m_vertexBuffer = m_direct3D.createVertexBuffer(vertices);

    // DirectXにTitleSceneのバッファを転送
    m_direct3D.setVertexBuffer(m_vertexBuffer);
}

std::vector<Vertex> BackgroundRenderer::createVertices() const
{
	const std::vector<Vertex> vertices{
        // 頂点1:左下
        {
            { -1.0f, -1.0f, 0.0f },
            { 1.0f, 1.0f, 1.0f, 1.0f },
            { 0.0f, 1.0f }
        },
        // 頂点2:左上
        {
            { -1.0f, 1.0f, 0.0f },
            { 1.0f, 1.0f, 1.0f, 1.0f },
            { 0.0f, 0.0f }
        },
        // 頂点3:右上
        {
            { 1.0f, 1.0f, 0.0f },
            { 1.0f, 1.0f, 1.0f, 1.0f },
            { 1.0f, 0.0f }
        },

        // 頂点4:左下
        {
            { -1.0f, -1.0f, 0.0f },
            { 1.0f, 1.0f, 1.0f, 1.0f },
            { 0.0f, 1.0f }
        },
        // 頂点5:右上
        {
            { 1.0f, 1.0f, 0.0f },
            { 1.0f, 1.0f, 1.0f, 1.0f },
            { 1.0f, 0.0f }
        },
        // 頂点6:右下
        {
            { 1.0f, -1.0f, 0.0f },
            { 1.0f, 1.0f, 1.0f, 1.0f },
            { 1.0f, 1.0f }
        },
	};

	return vertices;
}

void BackgroundRenderer::draw() const
{
	// 描画コマンド実行
	m_direct3D.draw(m_vertexCount);
}