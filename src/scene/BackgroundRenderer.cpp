// BackgroundRenderer class

# include "BackgroundRenderer.hpp"

# include "../dx11/Direct3D.hpp"

using namespace Util;
using namespace FilePath;
using namespace SceneSettings;

BackgroundRenderer::BackgroundRenderer(const SceneSettings::Scene& currentScene)
	: m_direct3D{ Direct3D::GetInstance() }
	, m_vertexCount{ 0 }
	, m_vertexBuffer{ nullptr }

    , m_titleTexture{ TitleImagePath }
    , m_clearTexture{ ClearImagePath }
    , m_overTexture{ OverImagePath }
{
	initialize(currentScene);
}

void BackgroundRenderer::initialize(const SceneSettings::Scene& currentScene)
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
    switch (currentScene)
    {
    case Scene::Title:
        m_direct3D.setTexture(m_titleTexture.getShaderResourceView());
        break;

    case Scene::Clear:
        m_direct3D.setTexture(m_clearTexture.getShaderResourceView());
        break;

    case Scene::Over:
        m_direct3D.setTexture(m_overTexture.getShaderResourceView());
        break;

    default:
        break;
    }
    

    // 定数バッファを設定
    ObjectConstants constants{};

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