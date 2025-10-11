// BasicVertexShader: 頂点シェーダー
// 頂点の位置と色をGPUの出力に変換する

// テクスチャとサンプラーを宣言
Texture2D txTexture : register(t0);     // t0スロットのテクスチャリソース
SamplerState smpSampler : register(s0); // s0スロットのサンプラーリソース

struct VS_INPUT
{
    float3 pos : POSITION;              // 頂点の位置 (x, y, z)
    float4 color : COLOR;               // 頂点の色 (r, g, b, a)
    float2 uv : TEXCOORD;              // uv座標(u, v)
};

struct PS_INPUT
{
    float4 pos : SV_POSITION;           // 出力位置 (システム予約セマンティクス)
    float4 color : COLOR;               // ピクセルシェーダーに渡す色
    float2 uv : TEXCOORD;              // uv座標(u, v)
};

PS_INPUT VSMain(VS_INPUT input)
{
    PS_INPUT output;
    
    // 位置をそのままクリップ空間に出力（MapRendererでクリップ空間座標を設定するため）
    output.pos = float4(input.pos, 1.0f);
    
    // 色をピクセルシェーダーに渡す
    output.color = input.color;
    
    // テクスチャ座標をピクセルシェーダーに渡す
    output.uv = input.uv;
    
    return output;
}

float4 PSMain(PS_INPUT input) : SV_TARGET
{
    // テクスチャをサンプリングして色を取得
    float4 textureColor = txTexture.Sample(smpSampler, input.uv);
    
    // 受け取った色とサンプリングした色を乗算して返す
    return input.color * textureColor;
}