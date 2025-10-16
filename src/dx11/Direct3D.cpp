// Direct3D class

# include "Direct3D.hpp"

using namespace Config;

Direct3D::Direct3D()
    : m_hWnd{ nullptr }
    , m_device{ nullptr }
    , m_deviceContext{ nullptr }
    , m_swapChain{ nullptr }
    , m_renderTargetView{ nullptr }
    , m_vertexShader{ nullptr }
    , m_pixelShader{ nullptr }
    , m_inputLayout{ nullptr }
    , m_compiledVertexShader{ nullptr }
    , m_samplerState{ nullptr }
    , m_blendState{ nullptr }
{

}

Direct3D& Direct3D::GetInstance()
{
    // 静的インスタンスを保持し、返す
	static Direct3D instance;
	return instance;
}

bool Direct3D::initialize(const HWND& hWnd)
{
    m_hWnd = hWnd;

    // スワップチェーン設定
    const DXGI_SWAP_CHAIN_DESC scDesc{ registerSwapChain() };

    // スワップチェーン作成
    const HRESULT swapChainResult{ createDeviceAndSwapChain(scDesc) };
    if (FAILED(swapChainResult))
    {
        // 失敗時、return
        MessageBox(m_hWnd, L"Direct3D: デバイスの作成に失敗しました", L"DirectX エラー", MB_ICONERROR);
        return false;
    }

    // レンダーターゲットビュー作成
    const HRESULT renderTargetViewResult{ createRenderTargetView(scDesc) };
    if (FAILED(renderTargetViewResult))
    {
        // 失敗時、return
        MessageBox(m_hWnd, L"Direct3D: レンダーターゲットビューの作成に失敗しました", L"DirectX エラー", MB_ICONERROR);
        return false;
    }

    // ビューポート設定
    setViewPortSettings();

    // シェーダーのコンパイル/作成
    if (!compileShader())
    {
        // 失敗時、return
        MessageBox(m_hWnd, L"Direct3D: シェーダーの作成に失敗しました。", L"DirectX エラー", MB_ICONERROR);
        return false;
    }
    
    // インプットレイアウトの作成
    const HRESULT inputLayoutResult{ createInputLayout() };
    if (FAILED(inputLayoutResult))
    {
        // 失敗時、return
        MessageBox(m_hWnd, L"Direct3D: インプットレイアウトの作成に失敗しました。", L"DirectX エラー", MB_ICONERROR);
        return false;
    }

    // サンプラーステート作成
    const HRESULT samplerResult{ createSamplerState() };
    if (FAILED(samplerResult))
    {
        // 失敗時、return
        MessageBox(m_hWnd, L"Direct3D: サンプラーステートの作成に失敗しました。", L"DirectX エラー", MB_ICONERROR);
        return false;
    }

    // ブレンドステート作成
    const HRESULT blendResult{ createBlendState() };
    if (FAILED(blendResult))
    {
        // 失敗時、return
        MessageBox(m_hWnd, L"Direct3D: ブレンドステートの作成に失敗しました。", L"DirectX エラー", MB_ICONERROR);
        return false;
    }

    // パイプライン設定
    setRenderPipeline();

    // 全てが正常終了時、trueを返す
	return true;
}

ComPtr<ID3D11Buffer> Direct3D::createVertexBuffer(const std::vector<Vertex>& vertices)
{
    // バッファの仕様を設定する
    D3D11_BUFFER_DESC bufferDesc{};
    bufferDesc.Usage = D3D11_USAGE_DEFAULT;
    bufferDesc.ByteWidth = sizeof(Vertex) * static_cast<UINT>(vertices.size()); // バイトサイズを計算(構造体のバイト数 * 要素数)
    bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bufferDesc.MiscFlags = 0;
    bufferDesc.CPUAccessFlags = 0;

    // 頂点バッファ
    ComPtr<ID3D11Buffer> vertexBuffer;

    // 書き込むデータ
    D3D11_SUBRESOURCE_DATA initData{};
    initData.pSysMem = vertices.data();

    // 頂点バッファ作成
    const HRESULT vertexBufferResult{ m_device->CreateBuffer(&bufferDesc, &initData, vertexBuffer.GetAddressOf()) };
    if (FAILED(vertexBufferResult))
    {
        // 失敗時、return
        MessageBox(m_hWnd, L"Direct3D: 頂点バッファの作成に失敗しました。", L"DirectX エラー", MB_ICONERROR);
        return nullptr;
    }

    return vertexBuffer;
}

void Direct3D::updateVeretexBuffer(const ComPtr<ID3D11Buffer>& vertexBuffer, const std::vector<Vertex>& vertices)
{
    // UpdateSubresourceを使ってGPU上のバッファの内容を更新
    m_deviceContext->UpdateSubresource(
        vertexBuffer.Get(),
        0,                  // Subresource
        nullptr,            // pBox (全体を更新するのでnullptr)
        vertices.data(),    // pSrcData (ソースデータ)
        0,                  // SrcRowPitch (未使用なので0)
        0                   // SrcDepthPitch (未使用なので0)
    );
}

void Direct3D::setVertexBuffer(const ComPtr<ID3D11Buffer>& vertexBuffer)
{
    // 頂点バッファをインプットアセンブラ(IA)ステージに設定
    const UINT stride{ sizeof(Vertex) }; // 1つの頂点データのサイズ
    const UINT offset{ 0 };

    // 頂点バッファのアドレス取得
    auto* buffer{ vertexBuffer.Get() };
    m_deviceContext->IASetVertexBuffers(
        0,							// 開始スロット
        1,							// バッファ数
        &buffer,
        &stride,
        &offset
    );
}

void Direct3D::setTexture(const ComPtr<ID3D11ShaderResourceView>& shaderResourceView) const
{
    // シェーダーリソースビューをピクセルシェーダーのt0スロットに設定
    m_deviceContext->PSSetShaderResources(0, 1, shaderResourceView.GetAddressOf());
}

void Direct3D::clearBackground(const DirectX::XMFLOAT4& backgroundColor) const
{
    // 背景のクリア
    m_deviceContext->ClearRenderTargetView(m_renderTargetView.Get(), (const float*)&backgroundColor);
}

void Direct3D::draw(const UINT vertexCount) const
{
    m_deviceContext->Draw(vertexCount, 0);
}

void Direct3D::flip() const
{
    // バックバッファの内容をフロント側に転送
    m_swapChain->Present(1, 0);
}

DXGI_SWAP_CHAIN_DESC Direct3D::registerSwapChain() const
{
	DXGI_SWAP_CHAIN_DESC swapChainDesc{};
	swapChainDesc.BufferCount = 1;                                  // バックバッファ(裏画面)の数
	swapChainDesc.BufferDesc.Width = WindowWidth;                   // 画面の幅
	swapChainDesc.BufferDesc.Height = WindowHeight;                 // 画面の高さ
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;   // 色情報の保持形式
	swapChainDesc.BufferDesc.RefreshRate.Numerator = RefreshRate;   // リフレッシュレート(分子)
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;           // リフレッシュレート(分母)
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;    // バックバッファ(裏画面)の使用用途
	swapChainDesc.OutputWindow = m_hWnd;                            // 出力先ウィンドウの識別ハンドル
	swapChainDesc.SampleDesc.Count = 1;                             // アンチエイリアシング設定(行なわない)
	swapChainDesc.SampleDesc.Quality = 0;                           // アンチエイリアシングの品質(使わないから0)
	swapChainDesc.Windowed = TRUE;                                  // ウィンドウモード
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;            // バックバッファ転送後、バックバッファをクリア

    return swapChainDesc;
}

HRESULT Direct3D::createDeviceAndSwapChain(const DXGI_SWAP_CHAIN_DESC& scDesc)
{
    const D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_11_1 };   // 使用するのはDirectX11.1
    const UINT numFeatureLevels = ARRAYSIZE(featureLevels);                 // 要求するバージョンの要素数
    const UINT createDeviceFlags = 0;                                       // デバイス作成における特殊なオプションは使用しない

    // デバイス作成呼び出し
    const HRESULT hResult{ D3D11CreateDeviceAndSwapChain(
        NULL,                                                               // 既定のグラフィックスカードを使用する
        D3D_DRIVER_TYPE_HARDWARE,                                           // 物理GPUを使用する
        NULL,                                                               // ソフトウェアGPUじゃないから、NULL
        createDeviceFlags,                                                  // デバイスの作成オプション
        featureLevels,                                                      // 要求機能リスト
        numFeatureLevels,                                                   // 要求機能の要素数
        D3D11_SDK_VERSION,                                                  // D3D11 SDKのバージョン指定
        &scDesc,                                                            // スワップチェーンのポインタ
        m_swapChain.GetAddressOf(),                                         // スワップチェーンの格納先
        m_device.GetAddressOf(),                                            // デバイスの格納先
        NULL,                                                               // デバイスの機能レベル情報は、使わない
        m_deviceContext.GetAddressOf()                                      // デバイスコンテキストの格納先
    ) };

    return hResult;
}

HRESULT Direct3D::createRenderTargetView(const DXGI_SWAP_CHAIN_DESC& scDesc)
{
    // スワップチェーンからバックバッファのポインターを取得
    ComPtr<ID3D11Texture2D> pBackBuffer;

    // バックバッファのリソース取得
    const HRESULT backBufferResult{ m_swapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer)) };
    if (FAILED(backBufferResult))
    {
        // 失敗時、return
        return backBufferResult;
    }

    // レンダーターゲット作成
    D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc{};
    renderTargetViewDesc.Format = scDesc.BufferDesc.Format;             // レンダーターゲットが参照する色情報の保持形式
    renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D; // リソースの種類(画像データとしてバックバッファにアクセスする)

    return m_device->CreateRenderTargetView(
        pBackBuffer.Get(),                                              // 描画先(今回はバックバッファ)
        &renderTargetViewDesc,                                          // レンダーターゲットビューの設定
        &m_renderTargetView);                                           // レンダーターゲットビューの格納先
}

void Direct3D::setViewPortSettings() const
{
    // バックバッファをレンダーターゲットに設定
    m_deviceContext->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), nullptr);

    // ビューポート設定
    const D3D11_VIEWPORT viewPort{ 0.0f, 0.0f, WindowWidth, WindowHeight, 0.0f, 1.0f };
    m_deviceContext->RSSetViewports(1, &viewPort);
}

bool Direct3D::compileShader()
{
    // 頂点シェーダーを読み込み＆コンパイル
    const HRESULT vertexShaderResult{ D3DCompileFromFile(
        ShaderFileName, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
        "VSMain", "vs_5_0", D3DCOMPILE_ENABLE_STRICTNESS, 0, &m_compiledVertexShader, nullptr)
    };

    if (FAILED(vertexShaderResult))
    {
        return false;
    }

    // ピクセルシェーダーを読み込み＆コンパイル
    ComPtr<ID3DBlob> compiledPixelShader{};
    const HRESULT pixelShaderResult{ D3DCompileFromFile(
        ShaderFileName, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
        "PSMain", "ps_5_0", D3DCOMPILE_ENABLE_STRICTNESS, 0, &compiledPixelShader, nullptr)
    };

    if (FAILED(pixelShaderResult))
    {
        return false;
    }

    // 頂点シェーダー作成
    if (FAILED(m_device->CreateVertexShader(m_compiledVertexShader->GetBufferPointer(), m_compiledVertexShader->GetBufferSize(), nullptr, &m_vertexShader)))
    {
        return false;
    }

    // ピクセルシェーダー作成
    if (FAILED(m_device->CreatePixelShader(compiledPixelShader->GetBufferPointer(), compiledPixelShader->GetBufferSize(), nullptr, &m_pixelShader)))
    {
        return false;
    }

    return true;
}

HRESULT Direct3D::createInputLayout()
{
    D3D11_INPUT_ELEMENT_DESC layoutDesc[]{
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
    };

    // 頂点インプットレイアウト作成
    return m_device->CreateInputLayout(layoutDesc, ARRAYSIZE(layoutDesc), m_compiledVertexShader->GetBufferPointer(), m_compiledVertexShader->GetBufferSize(), m_inputLayout.GetAddressOf());
}

HRESULT Direct3D::createSamplerState()
{
    D3D11_SAMPLER_DESC samplerDesc{};
    samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR; // 線形補間を使う最も一般的なフィルター
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;    // UV座標が0-1の範囲外の場合、テクスチャを繰り返す
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    samplerDesc.MinLOD = 0;
    samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

    // サンプラーステートを作成
    return m_device->CreateSamplerState(&samplerDesc, m_samplerState.GetAddressOf());
}

HRESULT Direct3D::createBlendState()
{
    D3D11_BLEND_DESC blendDesc{};

    // すべてのレンダーターゲットに対して同じ設定を適用する
    blendDesc.RenderTarget[0].BlendEnable = TRUE;                                     // ブレンドを有効化
    blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;                       // ソースカラーの乗算係数: ソースのアルファ値 (A_src)
    blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;                  // デスティネーションカラーの乗算係数: 1 - ソースのアルファ値 (1 - A_src)
    blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;                           // ブレンド演算: 加算
    blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;                        // ソースアルファの乗算係数 (通常はONE)
    blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;                      // デスティネーションアルファの乗算係数 (通常はZERO)
    blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;                      // アルファブレンド演算: 加算
    blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;   // すべてのカラーチャンネル (RGBA) への書き込みを有効にする

    // ブレンドステートを作成
    return m_device->CreateBlendState(&blendDesc, m_blendState.GetAddressOf());
}

void Direct3D::setRenderPipeline() const
{
    // シェーダーを設定
    m_deviceContext->VSSetShader(m_vertexShader.Get(), nullptr, 0);
    m_deviceContext->PSSetShader(m_pixelShader.Get(), nullptr, 0);

    // サンプラーステートをピクセルシェーダーに設定
    m_deviceContext->PSSetSamplers(0, 1, m_samplerState.GetAddressOf());

    // ブレンドステートを設定
    const FLOAT blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };                        // 使用しないため、すべて0.0f
    m_deviceContext->OMSetBlendState(m_blendState.Get(), blendFactor, 0xffffffff);  // 0xffffffff はサンプルマスク (通常はすべて有効)

    // インプットレイアウトを設定
    m_deviceContext->IASetInputLayout(m_inputLayout.Get());

    // 描画するプリミティブ（図形の種類）を三角形リストに設定
    m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

ComPtr<ID3D11Device> Direct3D::getDevice() const
{
    return m_device;
}