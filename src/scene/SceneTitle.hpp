// SceneTitle class

# pragma once

# include "SceneBase.hpp"
# include "SceneManager.hpp"
# include "BackgroundRenderer.hpp"

class SceneTitle : public SceneBase
{
public:

    // コンストラクタ
    SceneTitle();

    // デストラクタ
    ~SceneTitle();

private:

    // 更新処理
    void update() override;

    // 描画処理
    void draw() const override;

    // 背景描画クラス
    BackgroundRenderer backgroundRenderer;
};