// BaseScene class

# pragma once

# include <memory>
# include "../core/Config.hpp"
# include "../dx11/Direct3D.hpp"

class BaseScene
{
public:

    // コンストラクタ
    BaseScene();

    // デストラクタ
    virtual ~BaseScene() = default;

    // 初期化処理
    bool initialize();

    // シーンの更新処理
    virtual void updateScene() = 0;

    // シーンの描画処理
    void drawScene() const;

protected:

    // Direct3Dクラスのインスタンス
    Direct3D& m_direct3D;

    // 背景色
    ColorF m_backgroundColor;

    // 頂点バッファ
    ComPtr<ID3D11Buffer> m_vertexBuffer;

    // 頂点数
    UINT m_vertexCount;

private:

    // 頂点情報の作成処理
    virtual std::vector<Vertex> createVertices() const = 0;

    // 描画処理
    virtual void draw() const = 0;
};