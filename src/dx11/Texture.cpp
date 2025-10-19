// Texture class

# include "Texture.hpp"

Texture::Texture(const WCHAR* filePath)
	: shaderResourceView{ nullptr }
	, textureData{}
{
    loadTexture(filePath);
}

bool Texture::loadTexture(const WCHAR* filePath)
{
    // 画像を読み込む
    auto image{ std::make_unique<DirectX::ScratchImage>() };
    const HRESULT loadResult{ DirectX::LoadFromWICFile(filePath, DirectX::WIC_FLAGS_NONE, &textureData, *image) };
    if (FAILED(loadResult))
    {
        // 失敗
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

    return true;
}

Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> Texture::getShaderResourceView() const
{
    return shaderResourceView;
}