// BackgroundRenderer class

# pragma once

# include "../dx11/DirectX.hpp"

class BackgroundRenderer
{
public:

	// コンストラクタ
	BackgroundRenderer();

	// 描画処理
	void draw() const;

private:

	// 初期化処理
	void initialize();

	// 頂点情報の作成処理
	std::vector<Util::Vertex> createVertices() const;

	// Direct3Dクラスのインスタンス
	Direct3D& m_direct3D;

	// 頂点数
	UINT m_vertexCount;

	// 頂点バッファ
	ComPtr<ID3D11Buffer> m_vertexBuffer;
};