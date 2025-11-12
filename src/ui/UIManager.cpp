// UIManager class

# include "UIManager.hpp"
# include "../unit/UnitBase.hpp"
# include "../unit/UnitManager.hpp"
# include "../dx11/Direct3D.hpp"
# include "../util/InputState.hpp"

using namespace Util;
using namespace FilePath;
using namespace Config::UISettings;

UIManager::UIManager()
	: m_direct3D{ Direct3D::GetInstance() }
	, m_commandUITexture{ CommandUIPath }
	, m_commandUIVertexCount{ 0 }
	, m_commandUIBuffer{ nullptr }
    
    , m_highlightTexture{ HighlightTexturePath }
    , m_highlightVertexCount{ 0 }
    , m_highlightBuffer{ nullptr }

    , m_mouseOnUI{ false }
    , m_selectingCommand{ Command::None }

    , isDrawingCommandUI{ false }
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
    {
        // 頂点情報の作成
        const std::vector<Vertex> vertices{ createCommandUIVertices() };

        // 頂点数の計算
        m_commandUIVertexCount = static_cast<UINT>(vertices.size());

        // バッファの作成
        m_commandUIBuffer = m_direct3D.createVertexBuffer(vertices);
    }

    {
        // 頂点情報の作成
        const std::vector<Vertex> vertices{ createHighlightVertices() };

        // 頂点数の計算
        m_highlightVertexCount = static_cast<UINT>(vertices.size());

        // バッファの作成
        m_highlightBuffer = m_direct3D.createVertexBuffer(vertices);
    }
}

std::vector<Util::Vertex> UIManager::createCommandUIVertices() const
{
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
            { CommandUILeft, CommandUIBottom, 0.0f },
            defaultColor,
            uvBottomLeft
        },
        // 頂点2:左上
        {
            { CommandUILeft, CommandUITop, 0.0f },
            defaultColor,
            uvTopLeft
        },
        // 頂点3:右上
        {
            { CommandUIRight, CommandUITop, 0.0f },
            defaultColor,
            uvTopRight
        },

        // 頂点4:左下
        {
            { CommandUILeft, CommandUIBottom, 0.0f },
            defaultColor,
            uvBottomLeft
        },
        // 頂点5:右上
        {
            { CommandUIRight, CommandUITop, 0.0f },
            defaultColor,
            uvTopRight
        },
        // 頂点6:右下
        {
            { CommandUIRight, CommandUIBottom, 0.0f },
            defaultColor,
            uvBottomRight
        },
    };
    return vertices;
}

std::vector<Util::Vertex> UIManager::createHighlightVertices() const
{
    // グリッド位置に応じて各辺の座標を計算
    const float top{ CommandUITop - static_cast<int>(m_selectingCommand) * UIHighlightHeight };
    const float bottom{ top - UIHighlightHeight };

    // uv座標定義
    const DirectX::XMFLOAT2 uvTopLeft{ 0, 0 };		// 左上
    const DirectX::XMFLOAT2 uvBottomLeft{ 0, 1 };	// 左下
    const DirectX::XMFLOAT2 uvTopRight{ 1, 0 };		// 右上
    const DirectX::XMFLOAT2 uvBottomRight{ 1, 1 };	// 右下

    // 色は半透明の白
    const DirectX::XMFLOAT4 defaultColor{ 1.0f, 1.0f, 1.0f, 0.6f };

    const std::vector<Vertex> vertices{
        // 頂点1:左下
        {
            { CommandUILeft, bottom, 0.0f },
            defaultColor,
            uvBottomLeft
        },
        // 頂点2:左上
        {
            { CommandUILeft, top, 0.0f },
            defaultColor,
            uvTopLeft
        },
        // 頂点3:右上
        {
            { CommandUIRight, top, 0.0f },
            defaultColor,
            uvTopRight
        },

        // 頂点4:左下
        {
            { CommandUILeft, bottom, 0.0f },
            defaultColor,
            uvBottomLeft
        },
        // 頂点5:右上
        {
            { CommandUIRight, top, 0.0f },
            defaultColor,
            uvTopRight
        },
        // 頂点6:右下
        {
            { CommandUIRight, bottom, 0.0f },
            defaultColor,
            uvBottomRight
        },
    };
    return vertices;
}

void UIManager::update()
{
    // マウスの座標を取得
    const Vec2 mousePosition{ InputState::mouseWorldPosition };

    // マウスの境界チェック
    m_mouseOnUI = (
        mousePosition.x > CommandUILeft && mousePosition.y < CommandUITop &&
        mousePosition.x < CommandUIRight && mousePosition.y > CommandUIBottom);

    if (m_mouseOnUI && isDrawingCommandUI)
    {
        // 選択中コマンドの算出
        Command currentSelectingCommandIndex{ static_cast<int>(std::floor((CommandUITop - mousePosition.y) / UIHighlightHeight)) };

        // 選択中コマンドが切り替わった時
        if (m_selectingCommand != currentSelectingCommandIndex)
        {
            m_selectingCommand = currentSelectingCommandIndex;

            // バッファ更新
            m_direct3D.updateVeretexBuffer(m_highlightBuffer, createHighlightVertices());
        }

        // UIがクリックされた時
        if (InputState::KeyDown(VK_LBUTTON))
        {
            // クリックされたことをUnitManagerに伝える
            UnitManager::GetInstance().onCommandSelected(m_selectingCommand);
        }
    }
}

void UIManager::draw() const
{
    if (isDrawingCommandUI)
    {
        {
            // 背景テクスチャのセット
            m_direct3D.setTexture(m_commandUITexture.getShaderResourceView());

            // DirectXにTitleSceneのバッファを転送
            m_direct3D.setVertexBuffer(m_commandUIBuffer);

            // 描画コマンド実行
            m_direct3D.draw(m_commandUIVertexCount);
        }

        if (m_mouseOnUI)
        {
            // 背景テクスチャのセット
            m_direct3D.setTexture(m_highlightTexture.getShaderResourceView());

            // DirectXにTitleSceneのバッファを転送
            m_direct3D.setVertexBuffer(m_highlightBuffer);

            // 描画コマンド実行
            m_direct3D.draw(m_highlightVertexCount);
        }
    }
}