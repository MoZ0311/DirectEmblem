// UIManager class

# include "UIManager.hpp"
# include "../unit/UnitBase.hpp"
# include "../dx11/Direct3D.hpp"

using namespace Util;
using namespace FilePath;

UIManager::UIManager()
	: m_direct3D{ Direct3D::GetInstance() }
	, m_commandUITexture{ CommandUIPath }
	, m_commandUIVertexCount{ 0 }
	, m_commandUIBuffer{ nullptr }
{
    initialize();
}

UIManager& UIManager::GetInstance()
{
	static UIManager instance;
	return instance;
}

void UIManager::initialize()
{
	// 頂点情報の作成
	const std::vector<Vertex> vertices{ createCommandUIVertices() };

	// 頂点数の計算
	m_commandUIVertexCount = static_cast<UINT>(vertices.size());

	// バッファの作成
	m_commandUIBuffer = m_direct3D.createVertexBuffer(vertices);
}

std::vector<Util::Vertex> UIManager::createCommandUIVertices() const
{
    // 頂点座標定義
    const float left{ 0.7f };
    const float right{ 0.98f };

    const float uiWidth{ right - left };
    const float uiHeight{ uiWidth * Config::AspectRatio };

    const float bottom{ -0.967f };
    const float top{ bottom + uiHeight };

    // uv座標定義
    const DirectX::XMFLOAT2 uvTopLeft{ 0, 0 };		// 左上
    const DirectX::XMFLOAT2 uvBottomLeft{ 0, 1 };	// 左下
    const DirectX::XMFLOAT2 uvTopRight{ 1, 0 };		// 右上
    const DirectX::XMFLOAT2 uvBottomRight{ 1, 1 };	// 右下

    // 色はそのまま(白)
    const DirectX::XMFLOAT4 defaultColor{ 1.0f, 1.0f, 1.0f, 1.0f };

    const std::vector<Vertex> vertices{
        // 頂点1:左下
        {
            { left, bottom, 0.0f },
            defaultColor,
            uvBottomLeft
        },
        // 頂点2:左上
        {
            { left, top, 0.0f },
            defaultColor,
            uvTopLeft
        },
        // 頂点3:右上
        {
            { right, top, 0.0f },
            defaultColor,
            uvTopRight
        },

        // 頂点4:左下
        {
            { left, bottom, 0.0f },
            defaultColor,
            uvBottomLeft
        },
        // 頂点5:右上
        {
            { right, top, 0.0f },
            defaultColor,
            uvTopRight
        },
        // 頂点6:右下
        {
            { right, bottom, 0.0f },
            defaultColor,
            uvBottomRight
        },
    };
    return vertices;
}

void UIManager::update()
{

}

void UIManager::draw() const
{
    if (isDrawingCommandUI)
    {
        // 背景テクスチャのセット
        m_direct3D.setTexture(m_commandUITexture.getShaderResourceView());

        // DirectXにTitleSceneのバッファを転送
        m_direct3D.setVertexBuffer(m_commandUIBuffer);

        // 描画コマンド実行
        m_direct3D.draw(m_commandUIVertexCount);
    }
}