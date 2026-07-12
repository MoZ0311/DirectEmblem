// SceneClear class

# pragma once

# include "SceneBase.hpp"
# include "BackgroundRenderer.hpp"

class SceneClear : public SceneBase
{
public:

    // コンストラクタ
    SceneClear();

    // デストラクタ
    ~SceneClear();

private:

    // 更新処理
    void update() override;

    // 描画処理
    void draw() const override;

    // 背景描画クラス
    BackgroundRenderer backgroundRenderer;
};