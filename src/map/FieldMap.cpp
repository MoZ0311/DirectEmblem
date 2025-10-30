// FieldMap class

# include "FieldMap.hpp"
# include "../unit/UnitManager.hpp"

using namespace Util;
using namespace FilePath;
using namespace Config;
using namespace Config::MapSettings;

FieldMap::FieldMap()
    : m_mapGrid{ CSVReader::ConvertToInteger(CSVReader::readCsvFile("assets/data/map_data.csv")) }
    , m_accessibleTileGrid{}
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
    // 侵入可能配列の初期化
    m_accessibleTileGrid.assign(MapHeight, std::vector<bool>(MapWidth, false));

    {
        // 頂点情報の作成
        const std::vector<Vertex> vertices{ createMapVertices() };

        // 頂点数の計算
        m_mapVertexCount = static_cast<UINT>(vertices.size());

        // バッファの作成
        m_mapVertexBuffer = m_direct3D.createVertexBuffer(vertices);
    }

    {
        // 移動範囲用の頂点情報を作成
        const std::vector<Vertex> moveRangeVertices{ createMoveRangeVertices() };

        // 移動範囲用頂点数の計算
        m_moveRangeVertexCount = static_cast<UINT>(moveRangeVertices.size());

        // 移動範囲用のバッファを作成
        m_moveRangeBuffer = m_direct3D.createVertexBuffer(moveRangeVertices);
    }

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

std::vector<Util::Vertex> FieldMap::createMoveRangeVertices() const
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

            // 移動の可否で色を設定(-1より大きければアクセス可能)
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
    // グリッド位置に応じて各辺の座標を計算
    const float left{ MapStartX + m_mouseGridPosition.x * TileWidth };
    const float right{ left + TileWidth };
    const float top{ MapStartY - m_mouseGridPosition.y * TileHeight };
    const float bottom{ top - TileHeight };

    const DirectX::XMFLOAT4 color{ 1.0f, 1.0f, 1.0f, 0.6f };    // 色
    const DirectX::XMFLOAT2 uv{ 0.5f, 0.5f };                   // 白対応するuv座標

    const std::vector<Vertex> vertices{
        // 頂点1:左下
        {
            { left, bottom, 0.0f }, color, uv
        },
        // 頂点2:左上
        {
            { left, top, 0.0f }, color, uv
        },
        // 頂点3:右上
        {
            { right, top, 0.0f }, color, uv
        },
        // 頂点4:左下
        {
            { left, bottom, 0.0f }, color, uv
        },
        // 頂点5:右上
        {
            { right, top, 0.0f }, color, uv
        },
        // 頂点6:右下
        {
            { right, bottom, 0.0f }, color, uv
        }
    };

    return vertices;
}

void FieldMap::update()
{
    // 移動範囲用の頂点バッファを更新
    updateMoveRangeBuffer();

    // マウスの座標を取得
    const Vec2 mousePosition{ InputState::mouseWorldPosition };

    // グリッド座標の計算
    const int gridX{ static_cast<int>(std::floor((mousePosition.x - MapStartX) / TileWidth)) };
    const int gridY{ static_cast<int>(std::floor((MapStartY - mousePosition.y) / TileHeight)) };

    // グリッドの境界チェック
    m_mouseOnMap = gridX >= 0 && gridX < MapWidth && gridY >= 0 && gridY < MapHeight;

    if (m_mouseOnMap)
    {
        // 範囲内の場合
        m_mouseGridPosition = { gridX, gridY };

        // ハイライト用の頂点バッファを更新
        updateHighlightBuffer();
    }
    else
    {
        // 範囲外の場合
        m_mouseGridPosition = { -1, -1 };
    }
}

void FieldMap::draw() const
{
    {
        // 背景テクスチャのセット
        m_direct3D.setTexture(m_mapTexture.getShaderResourceView());

        // DirectXにTitleSceneのバッファを転送
        m_direct3D.setVertexBuffer(m_mapVertexBuffer);

        // 描画コマンド実行
        m_direct3D.draw(m_mapVertexCount);
    }
    
    if (UnitManager::GetInstance().isUnitMoving)
    {
        // ハイライト用のテクスチャをセット
        m_direct3D.setTexture(m_highlightTexture.getShaderResourceView());

        // 移動範囲用のバッファを転送
        m_direct3D.setVertexBuffer(m_moveRangeBuffer);

        // 描画コマンド実行
        m_direct3D.draw(m_moveRangeVertexCount);
    }

    if (m_mouseOnMap)
    {
        // ハイライト用のテクスチャをセット
        m_direct3D.setTexture(m_highlightTexture.getShaderResourceView());

        // ハイライト用のバッファを転送
        m_direct3D.setVertexBuffer(m_highlightBuffer);

        // 描画コマンド実行
        m_direct3D.draw(m_highlightVertexCount);
    }
}

void FieldMap::updateMoveRangeBuffer()
{
    // Direct3DのupdateVeretexBufferで既存のバッファを更新
    m_direct3D.updateVeretexBuffer(m_moveRangeBuffer, createMoveRangeVertices());
}

void FieldMap::updateHighlightBuffer()
{
    // Direct3DのupdateVeretexBufferで既存のバッファを更新
    m_direct3D.updateVeretexBuffer(m_highlightBuffer, createHighlightVertices());
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

std::vector<std::vector<bool>> FieldMap::getAccessibleTileGrid() const
{
    return m_accessibleTileGrid;
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
}