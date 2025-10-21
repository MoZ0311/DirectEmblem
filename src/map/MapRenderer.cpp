// MapRenderer class

# include "MapRenderer.hpp"

using namespace Util;
using namespace FilePath;
using namespace Config;
using namespace Config::MapSettings;

MapRenderer::MapRenderer()
    : m_mapData{ CSVReader::ConvertToInteger(CSVReader::readCsvFile("assets/data/map_data.csv")) }
	, m_direct3D{ Direct3D::GetInstance() }
    , m_mapTexture{ TileSheetPath }
    , m_highlightTexture{ WhiteTexturePath }
	, m_vertexCount{ 0 }
    , m_highlightVertexCount{ 0 }
	, m_vertexBuffer{ nullptr }
    , m_highlightBuffer{ nullptr }
    , m_mouseGridPosition{ -1, -1 }
    , m_mouseOnMap{ false }
{
	initialize();
}

MapRenderer& MapRenderer::GetInstance()
{
    // 静的インスタンスを保持し、返す
    static MapRenderer instance;
    return instance;
}

void MapRenderer::initialize()
{
	// 頂点情報の作成
	const std::vector<Vertex> vertices{ createVertices() };

	// 頂点数の計算
	m_vertexCount = static_cast<UINT>(vertices.size());

	// バッファの作成
	m_vertexBuffer = m_direct3D.createVertexBuffer(vertices);

    // ハイライト用の頂点情報を作成
    const std::vector<Vertex> highlightVertices{ createHighlightVertices() };

    // ハイライト用頂点数の計算
    m_highlightVertexCount = static_cast<UINT>(highlightVertices.size());

    // ハイライト用のバッファを作成
    m_highlightBuffer = m_direct3D.createVertexBuffer(highlightVertices);
}

std::vector<Vertex> MapRenderer::createVertices() const
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
            const TileType currentMapTile{ m_mapData[y][x] };

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

std::vector<Vertex> MapRenderer::createHighlightVertices() const
{
    // グリッド位置に応じて各辺の座標を計算
    const float left{ MapStartX + m_mouseGridPosition.x * TileWidth };
    const float right{ left + TileWidth };
    const float top{ MapStartY - m_mouseGridPosition.y * TileHeight };
    const float bottom{ top - TileHeight };

    const DirectX::XMFLOAT4 color{ 1.0f, 1.0f, 1.0f, 0.6f };    // 色
    const DirectX::XMFLOAT2 uv{ 0.0f, 0.0f };                   // uv座標

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

void MapRenderer::update()
{
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

void MapRenderer::draw() const
{
    // 背景テクスチャのセット
    m_direct3D.setTexture(m_mapTexture.getShaderResourceView());

    // DirectXにTitleSceneのバッファを転送
    m_direct3D.setVertexBuffer(m_vertexBuffer);

	// 描画コマンド実行
	m_direct3D.draw(m_vertexCount);

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

void MapRenderer::updateHighlightBuffer()
{
    // Direct3DのupdateVeretexBufferで既存のバッファを更新
    m_direct3D.updateVeretexBuffer(m_highlightBuffer, createHighlightVertices());
}

GridPosition MapRenderer::getMouseGridPosition() const
{
    return m_mouseGridPosition;
}