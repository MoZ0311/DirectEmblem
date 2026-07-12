// BackgroundRenderer class

# pragma once

# include <vector>
# include "../dx11/Texture.hpp"
# include "../core/Config.hpp"

class Direct3D;

class BackgroundRenderer
{
public:

	// コンストラクタ
	BackgroundRenderer(const SceneSettings::Scene& currentScene);

	// デストラクタ
	~BackgroundRenderer() = default;

	// 描画処理
	void draw() const;

private:

	// 初期化処理
	void initialize(const SceneSettings::Scene& currentScene);

	// 頂点情報の作成処理
	std::vector<Util::Vertex> createVertices() const;

	// Direct3Dクラスのインスタンス
	Direct3D& m_direct3D;

	// テクスチャ情報
	Texture m_titleTexture;
	Texture m_clearTexture;
	Texture m_overTexture;

	// 頂点数
	UINT m_vertexCount;

	// 頂点バッファ
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_vertexBuffer;
};