// FieldMap class

# include "FieldMap.hpp"

# include "../unit/UnitManager.hpp"
# include "../dx11/Direct3D.hpp"
# include "../util/InputState.hpp"
# include "../util/CSVReader.hpp"

using namespace Util;
using namespace FilePath;
using namespace Config;
using namespace Config::MapSettings;
using namespace Config::UnitSettings;

FieldMap::FieldMap()
    : m_mapGrid{ CSVReader::ConvertToInteger(CSVReader::readCsvFile("assets/data/map_data.csv")) }
    , m_accessibleTileGrid{ MapHeight, std::vector<bool>(MapWidth, false) }
	, m_direct3D{ Direct3D::GetInstance() }
    , m_mapTexture{ TileSheetPath }
    , m_highlightTexture{ HighlightTexturePath }

	, m_mapVertexCount{ 0 }
    , m_moveRangeVertexCount{ 0 }
    , m_highlightVertexCount{ 0 }

	, m_mapVertexBuffer{ nullptr }
    , m_moveRangeBuffer{ nullptr }
    , m_highlightBuffer{ nullptr }

    , m_mouseGridPosition{ -1, -1 }
    , m_mouseOnMap{ false }
    , m_mouseOveredTile{ TileType::Grass }
{
	initialize();
}

FieldMap& FieldMap::GetInstance()
{
    // 静的インスタンスを保持し、返す
    static FieldMap instance;
    return instance;
}

void FieldMap::initialize()
{
    // マップの頂点バッファを設定
    {
        // 頂点情報の作成
        const std::vector<Vertex> vertices{ createMapVertices() };

        // 頂点数の計算
        m_mapVertexCount = static_cast<UINT>(vertices.size());

        // バッファの作成
        m_mapVertexBuffer = m_direct3D.createVertexBuffer(vertices);
    }

    // 移動範囲の頂点バッファを設定
    {
        // 移動範囲用の頂点情報を作成
        const std::vector<Vertex> moveRangeVertices{ createMoveRangeVertices() };

        // 移動範囲用頂点数の計算
        m_moveRangeVertexCount = static_cast<UINT>(moveRangeVertices.size());

        // 移動範囲用のバッファを作成
        m_moveRangeBuffer = m_direct3D.createVertexBuffer(moveRangeVertices);
    }

    // マップタイルのハイライト用頂点バッファの設定
    {
        // ハイライト用の頂点情報を作成
        const std::vector<Vertex> highlightVertices{ createHighlightVertices() };

        // ハイライト用頂点数の計算
        m_highlightVertexCount = static_cast<UINT>(highlightVertices.size());

        // ハイライト用のバッファを作成
        m_highlightBuffer = m_direct3D.createVertexBuffer(highlightVertices);
    }
}

std::vector<Vertex> FieldMap::createMapVertices() const
{
    // タイルシートの情報定義
    // 2 * 2で四つのタイルマップなので、uvの最大1.0の半分ずつ
    const float uvTileWidth{ 0.5f };                    // タイルのuv座標上の幅
    const float uvTileHeight{ 0.5f };                   // タイルのuv座標上の高さ

    // 色 (白(1.0, 1.0, 1.0, 1.0)に設定)
    const DirectX::XMFLOAT4 colorF{ 1.0f, 1.0f, 1.0f, 1.0f };

    // 空の頂点群を宣言
    std::vector<Vertex> vertices{};

    // 二重ループで頂点を作成
    for (int y{ 0 }; y < MapHeight; ++y)
    {
        for (int x{ 0 }; x < MapWidth; ++x)
        {
            // タイルの各辺の座標を計算
            const float left{ MapStartX + x * TileWidth };
            const float right{ left + TileWidth };
            const float top{ MapStartY - y * TileHeight };
            const float bottom{ top - TileHeight };

            // 現在のタイルを取得
            const TileType currentMapTile{ m_mapGrid[y][x] };

            // タイルとテクスチャの対応表を取得
            const DirectX::XMFLOAT2 uvIndex = TileUVMap.at(currentMapTile);

            // テクスチャアトラスのuv座標計算
            const float uvLeft{ uvIndex.x * uvTileWidth };
            const float uvRight{ uvLeft + uvTileWidth };

            const float uvTop{ uvIndex.y * uvTileHeight };
            const float uvBottom{ uvTop + uvTileHeight };

            // テクスチャ座標 (UV: 0.0～1.0)
            const DirectX::XMFLOAT2 uvTopLeft{ uvLeft, uvTop };
            const DirectX::XMFLOAT2 uvTopRight{ uvRight, uvTop };
            const DirectX::XMFLOAT2 uvBottomLeft{ uvLeft, uvBottom };
            const DirectX::XMFLOAT2 uvBottomRight{ uvRight, uvBottom };

            // 頂点1:左下
            vertices.push_back({ { left, bottom, 0.0f }, colorF, uvBottomLeft });

            // 頂点1:左上
            vertices.push_back({ { left, top, 0.0f }, colorF, uvTopLeft });

            // 頂点3:右上
            vertices.push_back({ { right, top, 0.0f }, colorF, uvTopRight });

            // 頂点4:左下
            vertices.push_back({ { left, bottom, 0.0f }, colorF, uvBottomLeft });

            // 頂点5:右上
            vertices.push_back({ { right, top, 0.0f }, colorF, uvTopRight });

            // 頂点6:右下
            vertices.push_back({ { right, bottom, 0.0f }, colorF, uvBottomRight });
        }
    }
	return vertices;
}

std::vector<Vertex> FieldMap::createMoveRangeVertices() const
{
    // 赤色(移動不可)
    const DirectX::XMFLOAT4 colorRed{ 1.0f, 0.0f, 0.0f, 0.5f };

    // 青色(移動可能)
    const DirectX::XMFLOAT4 colorBlue{ 0.0f, 0.0f, 1.0f, 0.5f };

    // 空の頂点群を宣言
    std::vector<Vertex> vertices{};

    // 二重ループで頂点を作成
    for (int y{ 0 }; y < MapHeight; ++y)
    {
        for (int x{ 0 }; x < MapWidth; ++x)
        {
            // タイルの各辺の座標を計算
            const float left{ MapStartX + x * TileWidth };
            const float right{ left + TileWidth };
            const float top{ MapStartY - y * TileHeight };
            const float bottom{ top - TileHeight };

            // テクスチャアトラスのuv座標計算
            const DirectX::XMFLOAT2 uv{ 0.5f, 0.5f };

            // 移動の可否で色を設定
            const bool canAccess{ m_accessibleTileGrid[y][x] };
            const DirectX::XMFLOAT4 tileColor{ canAccess ? colorBlue : colorRed };

            // 頂点1:左下
            vertices.push_back({ { left, bottom, 0.0f }, tileColor, uv });

            // 頂点1:左上
            vertices.push_back({ { left, top, 0.0f }, tileColor, uv });

            // 頂点3:右上
            vertices.push_back({ { right, top, 0.0f }, tileColor, uv });

            // 頂点4:左下
            vertices.push_back({ { left, bottom, 0.0f }, tileColor, uv });

            // 頂点5:右上
            vertices.push_back({ { right, top, 0.0f }, tileColor, uv });

            // 頂点6:右下
            vertices.push_back({ { right, bottom, 0.0f }, tileColor, uv });
        }
    }
    return vertices;
}

std::vector<Vertex> FieldMap::createHighlightVertices() const
{
    // 形状は常に TileWidth * TileHeight の四角形
    const float halfWidth{ TileWidth / 2.0f };
    const float halfHeight{ TileHeight / 2.0f };

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

void FieldMap::update()
{
    // マウスの座標を取得
    const Vec2 mousePosition{ InputState::mouseWorldPosition };

    // グリッド座標の計算
    const int gridX{ static_cast<int>(std::floor((mousePosition.x - MapStartX) / TileWidth)) };
    const int gridY{ static_cast<int>(std::floor((MapStartY - mousePosition.y) / TileHeight)) };

    const GridPosition currentMouseGridPosition{ gridX, gridY };

    // グリッドの境界チェック
    m_mouseOnMap = (
        currentMouseGridPosition.x >= 0 && currentMouseGridPosition.x < MapWidth &&
        currentMouseGridPosition.y >= 0 && currentMouseGridPosition.y < MapHeight);

    if (m_mouseOnMap)
    {
        // 範囲内の場合
        if (m_mouseGridPosition != currentMouseGridPosition)
        {
            m_mouseGridPosition = currentMouseGridPosition;

            // 重なったタイルの更新
            m_mouseOveredTile = static_cast<TileType>(m_mapGrid[m_mouseGridPosition.y][m_mouseGridPosition.x]);
        }
    }
    else
    {
        // 範囲外の場合
        m_mouseGridPosition = { -1, -1 };
    }
}

void FieldMap::draw() const
{
    // マップ全体の描画
    {
        // マップの定数バッファ情報
        ObjectConstants mapConstants{};

        // ワールド行列を設定
        DirectX::XMStoreFloat4x4(&mapConstants.worldMatrix, DirectX::XMMatrixIdentity());
        DirectX::XMStoreFloat4x4(&mapConstants.viewMatrix, DirectX::XMMatrixIdentity());
        DirectX::XMStoreFloat4x4(&mapConstants.projectionMatrix, DirectX::XMMatrixIdentity());

        // マップの色はフルカラー (テクスチャの色をそのまま使用)
        mapConstants.color = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

        // バッファの更新
        m_direct3D.updateConstantBuffer(mapConstants);

        // マップテクスチャのセット
        m_direct3D.setTexture(m_mapTexture.getShaderResourceView());

        // DirectXにバッファを転送
        m_direct3D.setVertexBuffer(m_mapVertexBuffer);

        // 描画コマンド実行
        m_direct3D.draw(m_mapVertexCount);
    }
    
    // 移動可能範囲を描画
    const UnitState currentUnitState{ UnitManager::GetInstance().currentUnitState };
    if (currentUnitState == UnitState::StandBy ||
        currentUnitState == UnitState::Moving)
    {
        // 移動範囲の定数バッファ情報
        ObjectConstants moveRangeConstants{};

        // ワールド行列を設定
        DirectX::XMStoreFloat4x4(&moveRangeConstants.worldMatrix, DirectX::XMMatrixIdentity());
        DirectX::XMStoreFloat4x4(&moveRangeConstants.viewMatrix, DirectX::XMMatrixIdentity());
        DirectX::XMStoreFloat4x4(&moveRangeConstants.projectionMatrix, DirectX::XMMatrixIdentity());

        // 移動範囲の色(頂点情報を使う為、デフォルト)
        moveRangeConstants.color = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

        // バッファの更新
        m_direct3D.updateConstantBuffer(moveRangeConstants);

        // 移動範囲用(ハイライトと共通)のテクスチャをセット
        m_direct3D.setTexture(m_highlightTexture.getShaderResourceView());

        // 移動範囲用のバッファを転送
        m_direct3D.setVertexBuffer(m_moveRangeBuffer);

        // 描画コマンド実行
        m_direct3D.draw(m_moveRangeVertexCount);
    }
    
    // ハイライトの描画
    if (m_mouseOnMap)
    {
        // タイルの中心座標を取得
        const DirectX::XMFLOAT2 highlightCenter{ gridToScreen(m_mouseGridPosition) };

        DirectX::XMMATRIX heighlightWorld{ DirectX::XMMatrixTranslation(
            highlightCenter.x,
            highlightCenter.y,
            0.0f // Z座標は描画順序を調整するために使用
        ) };

        // 定数バッファの設定
        ObjectConstants highlightConstants{};

        // 行列の設定
        DirectX::XMStoreFloat4x4(&highlightConstants.worldMatrix, DirectX::XMMatrixTranspose(heighlightWorld));
        DirectX::XMStoreFloat4x4(&highlightConstants.viewMatrix, DirectX::XMMatrixIdentity());
        DirectX::XMStoreFloat4x4(&highlightConstants.projectionMatrix, DirectX::XMMatrixIdentity());

        // ハイライトの色（半透明）
        highlightConstants.color = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 0.6f);
        
        // バッファの更新
        m_direct3D.updateConstantBuffer(highlightConstants);

        // ハイライト用のテクスチャをセット
        m_direct3D.setTexture(m_highlightTexture.getShaderResourceView());

        // ハイライト用のバッファを転送
        m_direct3D.setVertexBuffer(m_highlightBuffer);

        // 描画コマンド実行
        m_direct3D.draw(m_highlightVertexCount);
    }
}

GridPosition FieldMap::getMouseGridPosition() const
{
    return m_mouseGridPosition;
}

bool FieldMap::getMouseOnMap() const
{
    return m_mouseOnMap;
}

std::vector<std::vector<int>> FieldMap::getMapData() const
{
    return m_mapGrid;
}

DirectX::XMFLOAT2 FieldMap::gridToScreen(const GridPosition& gridPosition) const
{
    // グリッド座標をスクリーン座標に変換
    const float tileLeft{ MapStartX + (gridPosition.x * TileWidth) };
    const float tileTop{ MapStartY - (gridPosition.y * TileHeight) };

    // タイルの中心座標を計算
    const float centerX{ tileLeft + (TileWidth / 2.0f) };
    const float centerY{ tileTop - (TileHeight / 2.0f) };

    // X と Y の値を含む XMFLOAT2 を返す
    return DirectX::XMFLOAT2(centerX, centerY);
}

std::vector<std::vector<bool>> FieldMap::getAccessibleTileGrid() const
{
    return m_accessibleTileGrid;
}

TileType FieldMap::getMouseOveredTile() const
{
    return m_mouseOveredTile;
}

void FieldMap::setAccessibleTileGrid(const std::vector<std::vector<int>>& distanceGrid, int mobility)
{
    // 二重ループで侵入可否の配列を設定
    for (int y{ 0 }; y < MapHeight; ++y)
    {
        for (int x{ 0 }; x < MapWidth; ++x)
        {
            const bool canAccess{ distanceGrid[y][x] <= mobility };
            m_accessibleTileGrid[y][x] = canAccess;
        }
    }

    // 移動範囲用の頂点バッファを更新
    m_direct3D.updateVeretexBuffer(m_moveRangeBuffer, createMoveRangeVertices());
}