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

private:

    // 頂点バッファの作成処理
    std::vector<Vertex> createVertices() const override;

    // 更新処理
    void update() override;

    // 描画処理
    void draw() const override;
};