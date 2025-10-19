// MapRenderer class

# pragma once

# include "../core/Config.hpp"
# include "../dx11/DirectX.hpp"
# include "../util/CSVReader.hpp"

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
	std::vector<Util::Vertex> createVertices() const;

	// マップデータの二次元配列
	const std::vector<std::vector<int>> m_mapData;

	// Direct3Dクラスのインスタンス
	Direct3D& m_direct3D;

	// マップのテクスチャアトラス
	Texture m_mapTexture;

	// 頂点数
	UINT m_vertexCount;

	// 頂点バッファ
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_vertexBuffer;
};