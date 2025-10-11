// MapRenderer class

# pragma once

# include "../core/Config.hpp"
# include "../dx11/DirectX.hpp"

class MapRenderer
{
public:

	// コンストラクタ
	MapRenderer();

	// 更新処理
	void update();

	// 描画処理
	void draw() const;

private:

	// 初期化処理
	void initialize();

	// 頂点情報の作成処理
	std::vector<Vertex> createVertices() const;

	// マップデータの二次元配列
	const Config::MapSettings::TileType m_mapData[Config::MapSettings::MapHeight][Config::MapSettings::MapWidth];

	// Direct3Dクラスのインスタンス
	Direct3D& m_direct3D;

	// 頂点数
	UINT m_vertexCount;

	// 頂点バッファ
	ComPtr<ID3D11Buffer> m_vertexBuffer;
};