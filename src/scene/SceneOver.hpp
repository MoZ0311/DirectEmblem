// SceneOver class

# pragma once

# include "SceneBase.hpp"
# include "BackgroundRenderer.hpp"

class SceneOver : public SceneBase
{
public:

    // コンストラクタ
    SceneOver();

    // デストラクタ
    ~SceneOver();

private:

    // 更新処理
    void update() override;

    // 描画処理
    void draw() const override;

    // 背景描画クラス
    BackgroundRenderer backgroundRenderer;
};