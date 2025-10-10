#pragma once

// DirectX関連のヘッダを纏める

// Direct3Dのライブラリ
# pragma comment(lib, "d3d11.lib")
# pragma comment(lib, "dxgi.lib")
# pragma comment(lib, "d3dcompiler.lib")

// Direct3D関連のヘッダ
# include <d3d11.h>
# include <d3dcompiler.h>

// 数学ライブラリ
# include <directxmath.h>

// テクスチャライブラリ
# include <DirectXTex.h>

// ComPtrを使用できるようにする
# include <wrl/client.h>
using namespace Microsoft::WRL;

// 自作クラスのヘッダも纏める
# include "Direct3D.hpp"
# include "Texture.hpp"