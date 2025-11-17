// BackgroundRenderer class

# include "BackgroundRenderer.hpp"

# include "../dx11/Direct3D.hpp"

using namespace Util;
using namespace FilePath;

BackgroundRenderer::BackgroundRenderer()
	: m_direct3D{ Direct3D::GetInstance() }
	, m_vertexCount{ 0 }
	, m_vertexBuffer{ nullptr }
    , m_texture{ TitleImagePath }
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

    // 頂点バッファを送る
    m_direct3D.setVertexBuffer(m_vertexBuffer);

    // テクスチャの設定
    m_direct3D.setTexture(m_texture.getShaderResourceView());

    // 定数バッファを設定
    Util::ObjectConstants constants{};

    // ワールド・ビュー・プロジェクションすべてに単位行列を設定
    XMStoreFloat4x4(&constants.worldMatrix, DirectX::XMMatrixIdentity());
    XMStoreFloat4x4(&constants.viewMatrix, DirectX::XMMatrixIdentity());
    XMStoreFloat4x4(&constants.projectionMatrix, DirectX::XMMatrixIdentity());

    // 色を設定
    constants.color = { 1.0f, 1.0f, 1.0f, 1.0f };

    // バッファの更新
    m_direct3D.updateConstantBuffer(constants);
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