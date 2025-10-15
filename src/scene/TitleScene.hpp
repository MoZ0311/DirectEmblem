// TitleScene class

# pragma once

# include "BaseScene.hpp"
# include "SceneManager.hpp"
# include "BackgroundRenderer.hpp"

class TitleScene : public BaseScene
{
public:

    // コンストラクタ
    TitleScene();

    // デストラクタ
    ~TitleScene();

private:

    // 更新処理
    void update() override;

    // 描画処理
    void draw() const override;

    // 背景描画クラス
    BackgroundRenderer backgroundRenderer;
};