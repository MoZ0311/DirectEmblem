// GameScene class

# pragma once

# include "BaseScene.hpp"

class GameScene : public BaseScene
{
public:

    // コンストラクタ
    GameScene();

    // デストラクタ
    ~GameScene();

private:

    // 頂点バッファの作成処理
    std::vector<Vertex> createVertices() const override;

    // 更新処理
    void update() override;

    // 描画処理
    void draw() const override;
};