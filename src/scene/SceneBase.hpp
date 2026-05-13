// SceneBase class

# pragma once

# include <directxmath.h>

class Direct3D;

class SceneBase
{
public:

    // デストラクタ
    virtual ~SceneBase() = default;

    // シーンの更新処理
    void updateScene();

    // シーンの描画処理
    void drawScene() const;

protected:

    // コンストラクタ
    SceneBase();

    // Direct3Dクラスのインスタンス
    Direct3D& m_direct3D;

    // 背景色
    DirectX::XMFLOAT4 m_backgroundColor;

private:

    // 更新処理
    virtual void update() = 0;

    // 描画処理
    virtual void draw() const = 0;
};