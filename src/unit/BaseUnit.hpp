// BaseUnit class

#pragma once

# include "../dx11/DirectX.hpp"

class BaseUnit
{
public:

	// デストラクタ
	~BaseUnit() = default;

	// 更新処理
	void update();

	// 描画処理
	void draw() const;
	
protected:

	// コンストラクタ
	BaseUnit();

	// 初期化処理
	void initialize();

	// 頂点情報の作成処理
	std::vector<Util::Vertex> createVertices() const;

	// Direct3Dクラスのインスタンス
	Direct3D& m_direct3D;

	Texture m_unitIconTexture;

	// 頂点数
	UINT m_vertexCount;

	// 頂点バッファ
	ComPtr<ID3D11Buffer> m_vertexBuffer;

	// ユニットの座標(グリッド位置)
	Util::Point m_unitPoint;

	// 移動済みであるか
	bool m_hasMoved;

	// 行動済みであるか
	bool m_hasActed;
};