// Direct3D class

# pragma once

# pragma comment(lib, "d3d11.lib")
# pragma comment(lib, "dxgi.lib")
# pragma comment(lib, "d3dcompiler.lib")

# include <d3d11.h>
# include <d3dcompiler.h>
# include <directxmath.h>
# include <wrl/client.h>
# include <vector>
# include "../core/Config.hpp"

using namespace Microsoft::WRL;

class Direct3D
{
public:

	// シングルトンインスタンスの生成/取得
	static Direct3D& GetInstance();
	
	// Direct3Dの初期化処理
	bool initialize(HWND hWnd);

	// 描画処理
	void draw(UINT vertexCount, ColorF backgroundColor) const;

	// デバイスの取得
	ComPtr<ID3D11Device> getDevice() const;

	// デバイスコンテキスト取得
	ComPtr<ID3D11DeviceContext> getDeviceContext() const;

	ComPtr<ID3D11VertexShader> m_vertexShader;			// 頂点シェーダー
	ComPtr<ID3D11PixelShader> m_pixelShader;			// ピクセルシェーダー
	ComPtr<ID3D11InputLayout> m_inputLayout;			// インプットレイアウト

private:

	// コンストラクタ
	Direct3D();

	// スワップチェーンの設定
	DXGI_SWAP_CHAIN_DESC registerSwapChain() const;

	// スワップチェーン、デバイスの作成
	HRESULT createSwapChain(DXGI_SWAP_CHAIN_DESC& scDesc);

	// レンダーターゲットビューの作成
	HRESULT createRenderTargetView(DXGI_SWAP_CHAIN_DESC& scDesc);

	// ビューポートの設定
	void setViewPortSettings() const;

	// シェーダーのコンパイル
	bool compileShader();

	// 頂点バッファの作成
	HRESULT createVertexBuffer();

	// インプットレイアウト作成
	HRESULT createInputLayout();

	// 頂点バッファをパイプラインに流す処理
	void setRenderPipeline() const;

	// DirectX 11の主要インターフェース
	ComPtr<ID3D11Device> m_device;						// Direcr3Dデバイス
	ComPtr<ID3D11DeviceContext> m_deviceContext;		// デバイスコンテキスト
	ComPtr<IDXGISwapChain> m_swapChain;					// スワップチェイン
	ComPtr<ID3D11RenderTargetView> m_renderTargetView;	// レンダーターゲットビュー
	ComPtr<ID3D11Buffer> m_vertexBuffer;				// 頂点バッファ

	// 2D描画シェーダー
	ComPtr<ID3DBlob> m_compiledVertexShader;			// コンパイル済みの頂点シェーダー

	// ウィンドウの識別ハンドル
	HWND m_hWnd;
};