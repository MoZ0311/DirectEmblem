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

	// 侵入可能タイルの取得
	std::vector<std::vector<bool>> getAccessibleTileGrid() const;

	// 侵入可能タイルの設定
	void setAccessibleTileGrid(const std::vector<std::vector<int>>& distanceGrid, int mobility);

private:

	// コンストラクタ
	FieldMap();

	// コピーコンストラクタを削除
	FieldMap(const FieldMap&) = delete;

	// 初期化処理
	void initialize();

	// マップ用頂点情報の作成処理
	std::vector<Util::Vertex> createMapVertices() const;

	// 移動範囲用の頂点情報の作成処理
	std::vector<Util::Vertex> createMoveRangeVertices() const;

	// ハイライト用頂点情報の作成
	std::vector<Util::Vertex> createHighlightVertices() const;

	// 移動範囲用頂点情報バッファの更新
	void updateMoveRangeBuffer();

	// ハイライト用頂点バッファの更新
	void updateHighlightBuffer();

	// マップデータの二次元配列
	const std::vector<std::vector<int>> m_mapGrid;

	// 侵入可能タイルの二次元配列
	std::vector<std::vector<bool>> m_accessibleTileGrid;

	// Direct3Dクラスのインスタンス
	Direct3D& m_direct3D;

	// マップのテクスチャアトラス
	Texture m_mapTexture;

	// ハイライト用のテクスチャ
	Texture m_highlightTexture;

	// マップ用の頂点数
	UINT m_mapVertexCount;

	// 移動範囲用の頂点数
	UINT m_moveRangeVertexCount;

	// ハイライト描画用の頂点数
	UINT m_highlightVertexCount;

	// マップ用の頂点バッファ
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_mapVertexBuffer;

	// 移動範囲用の頂点バッファ
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_moveRangeBuffer;

	// ハイライト描画用の頂点バッファ
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_highlightBuffer;

	// マウスのグリッド座標
	Config::MapSettings::GridPosition m_mouseGridPosition;

	// マウスがマップ上にあるか
	bool m_mouseOnMap;
};