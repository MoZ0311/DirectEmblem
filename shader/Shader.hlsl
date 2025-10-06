// BasicVertexShader: 頂点シェーダー
// 頂点の位置と色をGPUの出力に変換する
struct VS_INPUT
{
    float3 pos : POSITION;      // 頂点の位置 (x, y, z)
    float4 color : COLOR;       // 頂点の色 (r, g, b, a)
};

struct PS_INPUT
{
    float4 pos : SV_POSITION;   // 出力位置 (システム予約セマンティクス)
    float4 color : COLOR;       // ピクセルシェーダーに渡す色
};

PS_INPUT VSMain(VS_INPUT input)
{
    PS_INPUT output;
    
    // 位置をそのままクリップ空間に出力（MapRendererでクリップ空間座標を設定するため）
    output.pos = float4(input.pos, 1.0f);
    
    // 色をピクセルシェーダーに渡す
    output.color = input.color;
    
    return output;
}

// BasicPixelShader: ピクセルシェーダー
// ピクセル（画面の点）の色を決定する
float4 PSMain(PS_INPUT input) : SV_TARGET
{
    // 受け取った色をそのまま出力色とする
    return input.color;
}