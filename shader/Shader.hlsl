// BasicVertexShader: 頂点シェーダー
// 頂点の位置と色をGPUの出力に変換する

cbuffer ObjectConstants : register(b0)
{
    // C++の Util::ObjectConstants 構造体と対応させる
    matrix WorldMatrix;
    matrix ViewMatrix;
    matrix ProjectionMatrix;
    
    float4 Color; // ユニットの色情報 (ハイライト/行動済の色)
}

struct VS_INPUT
{
    float3 pos : POSITION;              // 頂点の位置 (x, y, z)
    float4 color : COLOR;               // 頂点の色 (r, g, b, a)
    float2 uv : TEXCOORD;               // uv座標(u, v)
};

struct PS_INPUT
{
    float4 pos : SV_POSITION;           // 出力位置 (システム予約セマンティクス)
    float4 color : COLOR;               // ピクセルシェーダーに渡す色
    float2 uv : TEXCOORD;               // uv座標(u, v)
};

PS_INPUT VSMain(VS_INPUT input)
{
    PS_INPUT output;
    
    // WVP行列の計算
    matrix WVP = mul(WorldMatrix, mul(ViewMatrix, ProjectionMatrix));
    
    // 頂点のローカル位置をWVP行列で変換し、クリップ空間に出力
    output.pos = mul(float4(input.pos, 1.0f), WVP);
    
    // 頂点バッファの色と定数バッファの色を乗算して、最終色を決定
    output.color = input.color * Color;
    
    // テクスチャ座標をピクセルシェーダーに渡す
    output.uv = input.uv;
    
    return output;
}

// テクスチャとサンプラーを宣言
Texture2D txTexture : register(t0); // t0スロットのテクスチャリソース
SamplerState smpSampler : register(s0); // s0スロットのサンプラーリソース

float4 PSMain(PS_INPUT input) : SV_TARGET
{
    // テクスチャをサンプリングして色を取得
    float4 textureColor = txTexture.Sample(smpSampler, input.uv);
    
    // 受け取った色とサンプリングした色を乗算して返す
    return input.color * textureColor;
}