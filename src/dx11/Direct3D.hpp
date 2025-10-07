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
	bool initialize(const HWND& hWnd);

	// 頂点バッファの作成処理
	ComPtr<ID3D11Buffer> createVertexBuffer(const std::vector<Vertex>& vertices);

	// 頂点バッファのステージング処理
	void setVertexBuffer(const ComPtr<ID3D11Buffer>& vertexBuffer);

	// 画面のクリア処理
	void clearBackground(const ColorF& backgroundColor) const;

	// 画面の描画処理
	void draw(const UINT& vertexCount) const;

	// 画面の転送処理
	void flip() const;

private:

	// コンストラクタ
	Direct3D();

	// スワップチェーンの設定
	DXGI_SWAP_CHAIN_DESC registerSwapChain() const;

	// スワップチェーン、デバイスの作成
	HRESULT createDeviceAndSwapChain(const DXGI_SWAP_CHAIN_DESC& scDesc);

	// レンダーターゲットビューの作成
	HRESULT createRenderTargetView(const DXGI_SWAP_CHAIN_DESC& scDesc);

	// ビューポートの設定
	void setViewPortSettings() const;

	// シェーダーのコンパイル
	bool compileShader();

	// インプットレイアウト作成
	HRESULT createInputLayout();

	// 頂点バッファをパイプラインに流す処理
	void setRenderPipeline() const;

	// DirectX 11の主要インターフェース
	ComPtr<ID3D11Device> m_device;						// Direcr3Dデバイス
	ComPtr<ID3D11DeviceContext> m_deviceContext;		// デバイスコンテキスト
	ComPtr<IDXGISwapChain> m_swapChain;					// スワップチェイン
	ComPtr<ID3D11RenderTargetView> m_renderTargetView;	// レンダーターゲットビュー

	ComPtr<ID3D11VertexShader> m_vertexShader;			// 頂点シェーダー
	ComPtr<ID3D11PixelShader> m_pixelShader;			// ピクセルシェーダー
	ComPtr<ID3D11InputLayout> m_inputLayout;			// インプットレイアウト

	// 2D描画シェーダー
	ComPtr<ID3DBlob> m_compiledVertexShader;			// コンパイル済みの頂点シェーダー

	// ウィンドウの識別ハンドル
	HWND m_hWnd;
};