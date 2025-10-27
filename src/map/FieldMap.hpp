// FieldMap class

# pragma once

# include "../core/Config.hpp"
# include "../dx11/DirectX.hpp"
# include "../util/CSVReader.hpp"
# include "../util/InputState.hpp"

class FieldMap
{
public:

	// シングルトンインスタンスの生成/取得
	static FieldMap& GetInstance();

	// 更新処理
	void update();

	// 描画処理
	void draw() const;

	// マウスが重なったタイルの算出処理
	Config::MapSettings::GridPosition getMouseGridPosition() const;

	// マウスがマップ上にあるか
	bool getMouseOnMap() const;

	// 二次元配列の取得
	std::vector<std::vector<int>> getMapData() const;

private:

	// コンストラクタ
	FieldMap();

	// コピーコンストラクタを削除
	FieldMap(const FieldMap&) = delete;

	// 初期化処理
	void initialize();

	// 頂点情報の作成処理
	std::vector<Util::Vertex> createVertices() const;

	// ハイライト用頂点情報の作成
	std::vector<Util::Vertex> createHighlightVertices() const;

	// ハイライト用頂点バッファの更新
	void updateHighlightBuffer();

	// マップデータの二次元配列
	const std::vector<std::vector<int>> m_mapData;

	// Direct3Dクラスのインスタンス
	Direct3D& m_direct3D;

	// マップのテクスチャアトラス
	Texture m_mapTexture;

	// ハイライト用のテクスチャ
	Texture m_highlightTexture;

	// 頂点数
	UINT m_vertexCount;

	// ハイライト描画用の頂点数
	UINT m_highlightVertexCount;

	// 頂点バッファ
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_vertexBuffer;

	// ハイライト描画用の頂点バッファ
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_highlightBuffer;

	// マウスのグリッド座標
	Config::MapSettings::GridPosition m_mouseGridPosition;

	// マウスがマップ上にあるか
	bool m_mouseOnMap;
};