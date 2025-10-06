// Direct3D class

# include "Direct3D.hpp"

using namespace Config;

Direct3D::Direct3D()
    : m_hWnd{ nullptr }
    , m_device{ nullptr }
    , m_deviceContext{ nullptr }
    , m_swapChain{ nullptr }
    , m_renderTargetView{ nullptr }
    , m_vertexBuffer{ nullptr }
    , m_vertexShader{ nullptr }
    , m_pixelShader{ nullptr }
    , m_inputLayout{ nullptr }
    , m_compiledVertexShader{ nullptr }
{

}

Direct3D& Direct3D::GetInstance()
{
    // 静的インスタンスを保持し、返す
	static Direct3D instance;
	return instance;
}

bool Direct3D::initialize(HWND hWnd)
{
    m_hWnd = hWnd;

    // スワップチェーン設定
    DXGI_SWAP_CHAIN_DESC scDesc{ registerSwapChain() };

    // スワップチェーン作成
    const HRESULT swapChainResult{ createSwapChain(scDesc) };
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

    // 頂点バッファの作成
    const HRESULT vertexBufferResult{ createVertexBuffer() };
    if (FAILED(vertexBufferResult))
    {
        // 失敗時、return
        MessageBox(m_hWnd, L"Direct3D: 頂点バッファの作成に失敗しました。", L"DirectX エラー", MB_ICONERROR);
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

    // 全てが正常終了時、trueを返す
	return true;
}

void Direct3D::draw(UINT vertexCount, ColorF backgroundColor) const
{
    // パイプライン設定
    setRenderPipeline();

    // 背景のクリア
    m_deviceContext->ClearRenderTargetView(m_renderTargetView.Get(), backgroundColor.rgba);

    // 描画コマンド
    m_deviceContext->Draw(vertexCount, 0);

    // バックバッファの内容を画面に表示
    m_swapChain->Present(1, 0);
}

ComPtr<ID3D11Device> Direct3D::getDevice() const
{
    return m_device;
}

ComPtr<ID3D11DeviceContext> Direct3D::getDeviceContext() const
{
    return m_deviceContext;
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

HRESULT Direct3D::createSwapChain(DXGI_SWAP_CHAIN_DESC& scDesc)
{
    D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_11_1 }; // 使用するのはDirectX11.1
    UINT numFeatureLevels = ARRAYSIZE(featureLevels);               // 要求するバージョンの要素数
    UINT createDeviceFlags = 0;                                     // デバイス作成における特殊なオプションは使用しない

    // デバイス作成呼び出し
    const HRESULT hResult = D3D11CreateDeviceAndSwapChain(
        NULL,                                                       // 既定のグラフィックスカードを使用する
        D3D_DRIVER_TYPE_HARDWARE,                                   // 物理GPUを使用する
        NULL,                                                       // ソフトウェアGPUじゃないから、NULL
        createDeviceFlags,                                          // デバイスの作成オプション
        featureLevels,                                              // 要求機能リスト
        numFeatureLevels,                                           // 要求機能の要素数
        D3D11_SDK_VERSION,                                          // D3D11 SDKのバージョン指定
        &scDesc,                                                    // スワップチェーンのポインタ
        m_swapChain.GetAddressOf(),                                 // スワップチェーンの格納先
        m_device.GetAddressOf(),                                    // デバイスの格納先
        NULL,                                                       // デバイスの機能レベル情報は、使わない
        m_deviceContext.GetAddressOf()                              // デバイスコンテキストの格納先
    );

    return hResult;
}

HRESULT Direct3D::createRenderTargetView(DXGI_SWAP_CHAIN_DESC& scDesc)
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
    D3D11_VIEWPORT viewPort{ 0.0f, 0.0f, WindowWidth, WindowHeight, 0.0f, 1.0f };
    m_deviceContext->RSSetViewports(1, &viewPort);
}

bool Direct3D::compileShader()
{
    const WCHAR shaderFileName[]{ L"shader/Shader.hlsl" };

    // 頂点シェーダーを読み込み＆コンパイル
    const HRESULT vertexShaderResult{ D3DCompileFromFile(
        shaderFileName, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
        "VSMain", "vs_5_0", D3DCOMPILE_ENABLE_STRICTNESS, 0, &m_compiledVertexShader, nullptr)
    };

    if (FAILED(vertexShaderResult))
    {
        return false;
    }

    // ピクセルシェーダーを読み込み＆コンパイル
    ComPtr<ID3DBlob> compiledPixelShader;
    const HRESULT pixelShaderResult{ D3DCompileFromFile(
        shaderFileName, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
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

HRESULT Direct3D::createVertexBuffer()
{
    // 頂点データ設定
    Vertex vertices[]{
        // 頂点データ: { x, y, z, r, g, b, a }
        { -1.0f,  1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f }, // 左上 (赤)
        {  1.0f,  1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f }, // 右上 (緑)
        { -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f }, // 左下 (青)
        {  1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f }, // 右下 (黄)
    };

    // 頂点バッファ設定
    D3D11_BUFFER_DESC bufferDesc{};
    bufferDesc.Usage = D3D11_USAGE_DEFAULT;				// 作成するバッファの使用法
    bufferDesc.ByteWidth = sizeof(vertices);			// 作成するバッファのバイトサイズ
    bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;	// デバイスにバインドするときの種類(頂点バッファ、インデックスバッファ、定数バッファなど)
    bufferDesc.MiscFlags = 0;							// その他のフラグ
    bufferDesc.CPUAccessFlags = 0;

    // 初期データ設定
    D3D11_SUBRESOURCE_DATA initData{};
    initData.pSysMem = vertices;

    // 頂点バッファ作成
    return m_device->CreateBuffer(&bufferDesc, &initData, m_vertexBuffer.GetAddressOf());
}

HRESULT Direct3D::createInputLayout()
{
    D3D11_INPUT_ELEMENT_DESC layout[]{
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 0,                           D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };

    // 頂点インプットレイアウト作成
    return m_device->CreateInputLayout(layout, ARRAYSIZE(layout), m_compiledVertexShader->GetBufferPointer(), m_compiledVertexShader->GetBufferSize(), m_inputLayout.GetAddressOf());
}

void Direct3D::setRenderPipeline() const
{
    // 頂点バッファをインプットアセンブラ(IA)ステージに設定
    UINT stride = sizeof(Vertex); // 1つの頂点データのサイズ
    UINT offset = 0;
    m_deviceContext->IASetVertexBuffers(
        0,							// 開始スロット
        1,							// バッファ数
        m_vertexBuffer.GetAddressOf(),
        &stride,
        &offset
    );

    // シェーダーを設定
    m_deviceContext->VSSetShader(m_vertexShader.Get(), nullptr, 0);
    m_deviceContext->PSSetShader(m_pixelShader.Get(), nullptr, 0);

    // インプットレイアウトを設定
    m_deviceContext->IASetInputLayout(m_inputLayout.Get());

    // 描画するプリミティブ（図形の種類）を三角形ストリップに設定
    m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
}