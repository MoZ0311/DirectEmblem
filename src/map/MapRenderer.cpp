// MapRenderer class

# include "MapRenderer.hpp"

using namespace Config;
using namespace Config::MapSettings;

MapRenderer::MapRenderer()
	: m_mapData{ TileType::Grass }
	, m_direct3D{ Direct3D::GetInstance() }
	, m_vertexCount{ 0 }
	, m_vertexBuffer{ nullptr }
{
	initialize();
}

void MapRenderer::initialize()
{
	// 頂点情報の作成
	std::vector<Vertex> vertices{ createVertices() };

	// 頂点数の計算
	m_vertexCount = static_cast<UINT>(vertices.size());

	// バッファの作成
	m_vertexBuffer = m_direct3D.createVertexBuffer(vertices);
}

std::vector<Vertex> MapRenderer::createVertices() const
{
    // 縦横比を計算
    const float aspectRatio{ static_cast<float>(WindowWidth) / WindowHeight };

    const float tileWidth{ 0.1f };                      // タイルの幅(64px)
    const float tileHeight{ tileWidth * aspectRatio };  // タイルの高さ

    const float startX{ -tileWidth * MapWidth / 2 };    // 開始x座標
    const float startY{ tileHeight * MapHeight / 2 };   // 開始y座標

    // テクスチャ座標 (UV: 0.0～1.0)
    const DirectX::XMFLOAT2 uvTopLeft{ 0.0f, 0.0f };
    const DirectX::XMFLOAT2 uvTopRight{ 1.0f, 0.0f };
    const DirectX::XMFLOAT2 uvBottomLeft{ 0.0f, 1.0f };
    const DirectX::XMFLOAT2 uvBottomRight{ 1.0f, 1.0f };

    // 色 (テスト用に白(1.0, 1.0, 1.0, 1.0)に設定)
    const DirectX::XMFLOAT4 colorF{ 1.0f, 1.0f, 1.0f, 1.0f };

    // 空の頂点群を宣言
    std::vector<Vertex> vertices{};

    // 二重ループで頂点を作成
    for (int y = 0; y < MapHeight; ++y)
    {
        for (int x = 0; x < MapWidth; ++x)
        {
            // 各タイルの左上座標を計算 (NDC空間)
            const float left = startX + x * tileWidth;
            const float right = startX + (x + 1) * tileWidth;
            const float top = startY - y * tileHeight;
            const float bottom = startY - (y + 1) * tileHeight;

            // 正方形を2つの三角形(T1, T2)で構成し、三角形リストに追加

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
	// DirectXにTitleSceneのバッファを転送
	m_direct3D.setVertexBuffer(m_vertexBuffer);

	// 描画コマンド実行
	m_direct3D.draw(m_vertexCount);
}