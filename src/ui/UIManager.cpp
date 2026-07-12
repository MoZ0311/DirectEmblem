// UIManager class

# include "UIManager.hpp"
# include "../battle/BattleManager.hpp"
# include "../dx11/Direct3D.hpp"
# include "../map/FieldMap.hpp"
# include "../util/InputState.hpp"
# include "../unit/UnitManager.hpp"

using namespace Util;
using namespace FilePath;
using namespace Config::MapSettings;
using namespace Config::UISettings;
using namespace Config::UnitSettings;

UIManager::UIManager()
	: m_direct3D{ Direct3D::GetInstance() }

    , m_grassUITexture{ GrassUIPath }
    , m_forestUITexture{ ForestUIPath }
    , m_fenceUITexture{ FenceUIPath }
    , m_waterUITexture{ WaterUIPath }
    , m_tileUIVertexCount{ 0 }
    , m_tileUIBuffer{ nullptr }

    , m_playerUITexture{ PlayerUIPath }
    , m_playerHoodUITexture{ PlayerHoodUIPath }
    , m_enemyUITexture{ EnemyUIPath }
    , m_enemyHoodUITexture{ EnemyHoodUIPath }
    , m_unitUIVertexCount{ 0 }
    , m_unitUIBuffer{ nullptr }

	, m_commandUITexture{ CommandUIPath }
	, m_commandUIVertexCount{ 0 }
	, m_commandUIBuffer{ nullptr }
    
    , m_highlightTexture{ HighlightTexturePath }
    , m_highlightVertexCount{ 0 }
    , m_highlightBuffer{ nullptr }

    , m_mouseOnUI{ false }
    , m_selectingCommand{ Command::None }

    , isDrawingCommandUI{ false }
    , m_prevUnitType{ UnitType::Sword }
{
    initialize();
}

UIManager& UIManager::GetInstance()
{
	static UIManager instance;
	return instance;
}

void UIManager::resetState()
{
    m_mouseOnUI = false;
    m_selectingCommand = Command::None;
    isDrawingCommandUI = false;
}

void UIManager::initialize()
{
    // タイルUIの頂点バッファの設定
    {
        // 頂点情報の作成
        const std::vector<Vertex> vertices{ createTileUIVertices() };

        // 頂点数の計算
        m_tileUIVertexCount = static_cast<UINT>(vertices.size());

        // バッファの作成
        m_tileUIBuffer = m_direct3D.createVertexBuffer(vertices);
    }

    // ユニットUIの頂点バッファの設定
    {
        // 頂点情報の作成
        const std::vector<Vertex> vertices{ createUnitUIVertices() };

        // 頂点数の計算
        m_unitUIVertexCount = static_cast<UINT>(vertices.size());

        // バッファの作成
        m_unitUIBuffer = m_direct3D.createVertexBuffer(vertices);
    }

    // コマンドUIの頂点バッファの設定
    {
        // 頂点情報の作成
        const std::vector<Vertex> vertices{ createCommandUIVertices() };

        // 頂点数の計算
        m_commandUIVertexCount = static_cast<UINT>(vertices.size());

        // バッファの作成
        m_commandUIBuffer = m_direct3D.createVertexBuffer(vertices);
    }

    // コマンドハイライトの頂点バッファ設定
    {
        // 頂点情報の作成
        const std::vector<Vertex> vertices{ createHighlightVertices() };

        // 頂点数の計算
        m_highlightVertexCount = static_cast<UINT>(vertices.size());

        // バッファの作成
        m_highlightBuffer = m_direct3D.createVertexBuffer(vertices);
    }
}

std::vector<Vertex> UIManager::createTileUIVertices() const
{
    // uv座標定義
    const DirectX::XMFLOAT2 uvTopLeft{ 0, 0 };		// 左上
    const DirectX::XMFLOAT2 uvBottomLeft{ 0, 1 };	// 左下
    const DirectX::XMFLOAT2 uvTopRight{ 1, 0 };		// 右上
    const DirectX::XMFLOAT2 uvBottomRight{ 1, 1 };	// 右下

    // 色はそのまま(白)
    const DirectX::XMFLOAT4 defaultColor{ 1.0f, 1.0f, 1.0f, 1.0f };

    // 描画位置の設定
    const float left{ CommandUILeft };
    const float right{ CommandUIRight };
    const float top{ MapStartY };
    const float bottom{ top - TileUIHeight };

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

std::vector<Vertex> UIManager::createUnitUIVertices() const
{
    // uv座標定義
    const DirectX::XMFLOAT2 uvTopLeft{ 0, 0 };		// 左上
    const DirectX::XMFLOAT2 uvBottomLeft{ 0, 1 };	// 左下
    const DirectX::XMFLOAT2 uvTopRight{ 1, 0 };		// 右上
    const DirectX::XMFLOAT2 uvBottomRight{ 1, 1 };	// 右下

    // 色はそのまま(白)
    const DirectX::XMFLOAT4 defaultColor{ 1.0f, 1.0f, 1.0f, 1.0f };

    // 描画位置の設定
    const float left{ -CommandUIRight };
    const float right{ -CommandUILeft };
    const float top{ MapStartY };
    const float bottom{ CommandUIBottom };

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

std::vector<Vertex> UIManager::createCommandUIVertices() const
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

std::vector<Vertex> UIManager::createHighlightVertices() const
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
        const Command currentSelectingCommandIndex{ static_cast<int>(std::floor((CommandUITop - mousePosition.y) / UIHighlightHeight)) };

        // 選択中コマンドが切り替わった時
        if (m_selectingCommand != currentSelectingCommandIndex)
        {
            m_selectingCommand = currentSelectingCommandIndex;
        }

        // UIがクリックされた時
        if (InputState::KeyDown(VK_LBUTTON))
        {
            // クリックされたことをBattleManagerに伝える
            BattleManager::GetInstance().setSelectedCommand(m_selectingCommand);
        }
    }
}

void UIManager::draw()
{
    // タイルUIの描画
    {
        // UIの頂点バッファ情報
        ObjectConstants tileUIConstants{};

        // 単位行列を設定
        DirectX::XMStoreFloat4x4(&tileUIConstants.worldMatrix, DirectX::XMMatrixIdentity());
        DirectX::XMStoreFloat4x4(&tileUIConstants.viewMatrix, DirectX::XMMatrixIdentity());
        DirectX::XMStoreFloat4x4(&tileUIConstants.projectionMatrix, DirectX::XMMatrixIdentity());

        // テクスチャの色そのまま
        tileUIConstants.color = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

        // バッファの更新
        m_direct3D.updateConstantBuffer(tileUIConstants);

        // テクスチャのセット
        const TileType currentTileType{ FieldMap::GetInstance().getMouseOveredTile()};
        switch (currentTileType)
        {
        case TileType::Grass:
            m_direct3D.setTexture(m_grassUITexture.getShaderResourceView());
            break;

        case TileType::Forest:
            m_direct3D.setTexture(m_forestUITexture.getShaderResourceView());
            break;

        case TileType::Fence:
            m_direct3D.setTexture(m_fenceUITexture.getShaderResourceView());
            break;

        case TileType::Water:
            m_direct3D.setTexture(m_waterUITexture.getShaderResourceView());
            break;

        default:
            break;
        }
        

        // DirectXにバッファを転送
        m_direct3D.setVertexBuffer(m_tileUIBuffer);

        // 描画コマンド実行
        m_direct3D.draw(m_tileUIVertexCount);
    }

    // ユニットUIの描画
    {
        // UIの頂点バッファ情報
        ObjectConstants unitUIConstants{};

        // 単位行列を設定
        DirectX::XMStoreFloat4x4(&unitUIConstants.worldMatrix, DirectX::XMMatrixIdentity());
        DirectX::XMStoreFloat4x4(&unitUIConstants.viewMatrix, DirectX::XMMatrixIdentity());
        DirectX::XMStoreFloat4x4(&unitUIConstants.projectionMatrix, DirectX::XMMatrixIdentity());

        // テクスチャの色そのまま
        unitUIConstants.color = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

        // バッファの更新
        m_direct3D.updateConstantBuffer(unitUIConstants);

        // テクスチャのセット
        const GridPosition currentMousePosition{ FieldMap::GetInstance().getMouseGridPosition() };
        const UnitType currentUnitType{ UnitManager::GetInstance().getUnitTypeAtPosition(currentMousePosition) };

        if (currentUnitType == UnitType::Sword ||
            currentUnitType == UnitType::Bow ||
            currentUnitType == UnitType::Enemy ||
            currentUnitType == UnitType::EnemyHood)
        {
            m_prevUnitType = currentUnitType;
        }

        switch (m_prevUnitType)
        {
        case UnitType::Sword:
            m_direct3D.setTexture(m_playerUITexture.getShaderResourceView());
            break;

        case UnitType::Bow:
            m_direct3D.setTexture(m_playerHoodUITexture.getShaderResourceView());
            break;

        case UnitType::Enemy:
            m_direct3D.setTexture(m_enemyUITexture.getShaderResourceView());
            break;

        case UnitType::EnemyHood:
            m_direct3D.setTexture(m_enemyHoodUITexture.getShaderResourceView());
            break;

        default:
            break;
        }

        // DirectXにバッファを転送
        m_direct3D.setVertexBuffer(m_unitUIBuffer);

        // 描画コマンド実行
        m_direct3D.draw(m_unitUIVertexCount);
    }

    if (isDrawingCommandUI)
    {
        // コマンドUIの描画
        {
            // UIの定数バッファ情報
            ObjectConstants commandUIConstants{};

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
            // ハイライトの定数バッファ情報
            ObjectConstants highlightConstants{};

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
            if (m_selectingCommand == Command::Attack ||
                m_selectingCommand == Command::Wait)
            {
                highlightConstants.color = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 0.6f);
            }
            else
            {
                highlightConstants.color = DirectX::XMFLOAT4(0.3f, 0.3f, 0.3f, 0.6f);
            }

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