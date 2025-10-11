// Texture class

# pragma once

#include "DirectX.hpp"

struct Texture
{
	// コンストラクタ
	Texture();

	// 読み込み処理
	bool loadTexture(const WCHAR* fileName);

	// 画像の読み取りハンドル
	ComPtr<ID3D11ShaderResourceView> shaderResourceView;

	// 画像情報
	DirectX::TexMetadata textureData;
};