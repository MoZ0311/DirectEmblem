// BaseScene class

# pragma once

# include <memory>
# include "../core/Config.hpp"
# include "../dx11/Direct3D.hpp"

class BaseScene
{
public:

    // コンストラクタ
    BaseScene(const ComPtr<ID3D11Device>& device);

    // 初期化処理
    bool initialize(const ComPtr<ID3D11Device>& device);

    // 更新処理
    virtual void update() = 0;

    // 描画処理
    void drawScene() const;

    // 終了処理
    virtual void exit() = 0;

protected:

    // 背景色
    ColorF m_backgroundColor;

    ComPtr<ID3D11Buffer> m_vertexBuffer;

    // 頂点数
    UINT m_vertexCount;

private:

    // 頂点バッファの作成処理
    virtual HRESULT createVertexBuffer(const ComPtr<ID3D11Device>& device) = 0;

    // 描画処理
    virtual void draw() const = 0;
};