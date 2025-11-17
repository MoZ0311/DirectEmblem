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
    // コマンドUIの頂点バッファの設定
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
    // 形状は常に UIWidth * UIHighlightHeight
    const float halfWidth{ UIWidth / 2.0f };
    const float halfHeight{ UIHighlightHeight / 2.0f };

    const DirectX::XMFLOAT4 color{ 1.0f, 1.0f, 1.0f, 1.0f };    // 色はデフォルト
    const DirectX::XMFLOAT2 uv{ 0.5f, 0.5f };                   // 白いテクスチャの中央をとる

    const std::vector<Vertex> vertices{
        // 頂点1:左下
        {
            { -halfWidth, -halfHeight, 0.0f }, color, uv
        },
        // 頂点2:左上
        {
            { -halfWidth, halfHeight, 0.0f }, color, uv
        },
        // 頂点3:右上
        {
            { halfWidth, halfHeight, 0.0f }, color, uv
        },
        // 頂点4:左下
        {
            { -halfWidth, -halfHeight, 0.0f }, color, uv
        },
        // 頂点5:右上
        {
            { halfWidth, halfHeight, 0.0f }, color, uv
        },
        // 頂点6:右下
        {
            { halfWidth, -halfHeight, 0.0f }, color, uv
        }
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
        // コマンドUIの描画
        {
            // マップの定数バッファ情報
            Util::ObjectConstants commandUIConstants{};

            // ワールド行列を設定
            DirectX::XMStoreFloat4x4(&commandUIConstants.worldMatrix, DirectX::XMMatrixIdentity());
            DirectX::XMStoreFloat4x4(&commandUIConstants.viewMatrix, DirectX::XMMatrixIdentity());
            DirectX::XMStoreFloat4x4(&commandUIConstants.projectionMatrix, DirectX::XMMatrixIdentity());

            // テクスチャの色をそのまま使用
            commandUIConstants.color = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

            // バッファの更新
            m_direct3D.updateConstantBuffer(commandUIConstants);

            // UIテクスチャのセット
            m_direct3D.setTexture(m_commandUITexture.getShaderResourceView());

            // DirectXにバッファを転送
            m_direct3D.setVertexBuffer(m_commandUIBuffer);

            // 描画コマンド実行
            m_direct3D.draw(m_commandUIVertexCount);
        }

        // UIハイライトの描画
        if (m_mouseOnUI)
        {
            // マップの定数バッファ情報
            Util::ObjectConstants highlightConstants{};

            // ハイライトの描画座標を算出
            const float highlightCenterX{ (CommandUILeft + CommandUIRight) / 2 };
            const float highlightCenterY{ (CommandUITop - UIHighlightHeight / 2) - UIHighlightHeight * static_cast<int>(m_selectingCommand) };

            DirectX::XMMATRIX heighlightWorld{ DirectX::XMMatrixTranslation(
                highlightCenterX,
                highlightCenterY,
                0.0f // Z座標は描画順序を調整するために使用
            ) };

            // ワールド行列を設定
            DirectX::XMStoreFloat4x4(&highlightConstants.worldMatrix, DirectX::XMMatrixTranspose(heighlightWorld));
            DirectX::XMStoreFloat4x4(&highlightConstants.viewMatrix, DirectX::XMMatrixIdentity());
            DirectX::XMStoreFloat4x4(&highlightConstants.projectionMatrix, DirectX::XMMatrixIdentity());

            // テクスチャを半透明にする
            highlightConstants.color = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 0.6f);

            // バッファの更新
            m_direct3D.updateConstantBuffer(highlightConstants);

            // ハイライトテクスチャのセット
            m_direct3D.setTexture(m_highlightTexture.getShaderResourceView());

            // DirectXにバッファを転送
            m_direct3D.setVertexBuffer(m_highlightBuffer);

            // 描画コマンド実行
            m_direct3D.draw(m_highlightVertexCount);
        }
    }
}