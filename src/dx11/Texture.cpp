// Texture class

# include "Texture.hpp"

Texture::Texture()
	: shaderResourceView{ nullptr }
	, textureData{}
{

}

// 読み込み処理
bool Texture::loadTexture(const WCHAR* filePath)
{
    // 画像を読み込む
    auto image{ std::make_unique<DirectX::ScratchImage>() };
    const HRESULT loadResult{ DirectX::LoadFromWICFile(filePath, DirectX::WIC_FLAGS_NONE, &textureData, *image) };
    if (FAILED(loadResult))
    {
        textureData = {};
        return false;
    }

    // リソースとシェーダーリソースビューを作成
    const HRESULT resourceResult{
        DirectX::CreateShaderResourceView(
        Direct3D::GetInstance().getDevice().Get(), image->GetImages(), image->GetImageCount(),
        textureData, &shaderResourceView)
    };

    if (FAILED(resourceResult))
    {
        // 失敗
        textureData = {};
        return false;
    }

    // スロットにセット
    Direct3D::GetInstance().setTexture(shaderResourceView);

    return true;
}