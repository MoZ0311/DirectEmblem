// Direct3D class

# pragma once

# include "DirectX.hpp"
# include "../core/Config.hpp"

class Direct3D
{
public:

	// シングルトンインスタンスの生成/取得
	static Direct3D& GetInstance();
	
	// Direct3Dの初期化処理
	bool initialize(const HWND& hWnd);

	// 頂点バッファの作成処理
	ComPtr<ID3D11Buffer> createVertexBuffer(const std::vector<Util::Vertex>& vertices);

	// 頂点バッファの更新処理
	void updateVeretexBuffer(const ComPtr<ID3D11Buffer>& vertexBuffer, const std::vector<Util::Vertex>& vertices);

	// 頂点バッファのステージング処理
	void setVertexBuffer(const ComPtr<ID3D11Buffer>& vertexBuffer);

	// テクスチャの設定処理
	void setTexture(const ComPtr<ID3D11ShaderResourceView>& shaderResourceView) const;

	// 画面のクリア処理
	void clearBackground(const DirectX::XMFLOAT4& backgroundColor) const;

	// 画面の描画処理
	void draw(const UINT vertexCount) const;

	// 画面の転送処理
	void flip() const;

	// デバイスの取得処理
	ComPtr<ID3D11Device> getDevice() const;

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

	// サンプラーステート作成
	HRESULT createSamplerState();

	// ブレンドステート作成
	HRESULT createBlendState();

	// 頂点バッファをパイプラインに流す処理
	void setRenderPipeline() const;

	// ウィンドウの識別ハンドル
	HWND m_hWnd;

	// Direcr3Dデバイス
	ComPtr<ID3D11Device> m_device;

	// デバイスコンテキスト
	ComPtr<ID3D11DeviceContext> m_deviceContext;

	// スワップチェイン
	ComPtr<IDXGISwapChain> m_swapChain;

	// レンダーターゲットビュー
	ComPtr<ID3D11RenderTargetView> m_renderTargetView;

	// 頂点シェーダー
	ComPtr<ID3D11VertexShader> m_vertexShader;

	// ピクセルシェーダー
	ComPtr<ID3D11PixelShader> m_pixelShader;

	// インプットレイアウト
	ComPtr<ID3D11InputLayout> m_inputLayout;

	// コンパイル済みの頂点シェーダー
	ComPtr<ID3DBlob> m_compiledVertexShader;

	// サンプラーステート
	ComPtr<ID3D11SamplerState> m_samplerState;

	// ブレンドステート
	ComPtr<ID3D11BlendState> m_blendState;
};