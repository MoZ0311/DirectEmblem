// MapRenderer class

# include "MapRenderer.hpp"

using namespace Config;
using namespace Config::MapSettings;

MapRenderer::MapRenderer()
    : m_mapData{ CSVReader::ConvertToInteger(CSVReader::readCsvFile("assets/data/map_data.csv")) }
	, m_direct3D{ Direct3D::GetInstance() }
	, m_vertexCount{ 0 }
	, m_vertexBuffer{ nullptr }
{
	initialize();
}

void MapRenderer::initialize()
{
    // 背景テクスチャのロード
    Texture texture{ Config::TileSheetPath };

    // 背景テクスチャのセット
    m_direct3D.setTexture(texture.getShaderResourceView());

	// 頂点情報の作成
	std::vector<Vertex> vertices{ createVertices() };

	// 頂点数の計算
	m_vertexCount = static_cast<UINT>(vertices.size());

	// バッファの作成
	m_vertexBuffer = m_direct3D.createVertexBuffer(vertices);

    // DirectXにTitleSceneのバッファを転送
    m_direct3D.setVertexBuffer(m_vertexBuffer);
}

std::vector<Vertex> MapRenderer::createVertices() const
{
    const float startX{ -TileWidth * MapWidth / 2 };    // 開始x座標
    const float startY{ TileHeight * MapHeight / 2 };   // 開始y座標

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
            const float left{ startX + x * TileWidth };
            const float right{ left + TileWidth };
            const float top{ startY - y * TileHeight };
            const float bottom{ top - TileHeight };

            // 現在のタイルを取得
            const TileType currentMapTile{ m_mapData[y][x] };

            // テクスチャアトラスのuv座標計算
            const float uvLeft{ static_cast<int>(currentMapTile) % 2 * uvTileWidth };
            const float uvRight{ uvLeft + uvTileWidth };

            const float uvTop{ static_cast<int>(currentMapTile) >= 2 ? uvTileHeight : 0 };
            const float uvBottom{ uvTop + uvTileHeight };

            // テクスチャ座標 (UV: 0.0～1.0)
            const DirectX::XMFLOAT2 uvTopLeft{ uvLeft, uvTop };
            const DirectX::XMFLOAT2 uvTopRight{ uvRight, uvTop };
            const DirectX::XMFLOAT2 uvBottomLeft{ uvLeft, uvBottom };
            const DirectX::XMFLOAT2 uvBottomRight{ uvRight, uvBottom };

            // --- T1 (左下, 左上, 右上) ---

            // 頂点1:左下
            vertices.push_back({ { left, bottom, 0.0f }, colorF, uvBottomLeft });

            // 頂点1:左上
            vertices.push_back({ { left, top, 0.0f }, colorF, uvTopLeft });

            // 頂点3:右上
            vertices.push_back({ { right, top, 0.0f }, colorF, uvTopRight });


            // --- T2 (左下, 右上, 右下) ---

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

void MapRenderer::update()
{

}

void MapRenderer::draw() const
{
	// 描画コマンド実行
	m_direct3D.draw(m_vertexCount);
}