// BaseScene class

# pragma once

# include <memory>
# include <chrono>
# include "../core/Config.hpp"
# include "../dx11/Direct3D.hpp"

class BaseScene
{
public:

    // デストラクタ
    virtual ~BaseScene() = default;

    // 初期化処理
    bool initialize();

    // シーンの更新処理
    void updateScene();

    // シーンの描画処理
    void drawScene() const;

protected:

    // コンストラクタ
    BaseScene();

    // deltaTimeの取得処理
    float getDeltaTime() const;

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

    // 前フレームからの経過時間(deltaTime)の算出
    void calculateDeltaTime();

    // 更新処理
    virtual void update() = 0;

    // 描画処理
    virtual void draw() const = 0;

    // 直前の時間
    std::chrono::time_point<std::chrono::high_resolution_clock> m_prevTime;

    // 前フレームからの経過時間
    float m_deltaTime;
};