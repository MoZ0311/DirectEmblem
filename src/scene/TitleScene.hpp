// TitleScene class

# pragma once

# include "BaseScene.hpp"

class TitleScene : public BaseScene
{
public:

    // コンストラクタ
    TitleScene();

    // デストラクタ
    ~TitleScene();

    // 更新処理
    void updateScene() override;

private:

    // 頂点バッファの作成処理
    std::vector<Vertex> createVertices() const override;

    // 描画処理
    void draw() const override;
};