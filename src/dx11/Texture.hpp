// Texture class

# pragma once

# include "DirectX.hpp"

class Texture
{
public:

	// コンストラクタ
	Texture(const WCHAR* filePath);

	// シェーダーリソースビューの取得
	ComPtr<ID3D11ShaderResourceView> getShaderResourceView() const;

private:

	// 読み込み処理
	bool loadTexture(const WCHAR* filePath);

	// 画像の読み取りハンドル
	ComPtr<ID3D11ShaderResourceView> shaderResourceView;

	// 画像情報
	DirectX::TexMetadata textureData;
};