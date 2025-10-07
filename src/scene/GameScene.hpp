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

    // 更新処理
    void updateScene() override;

private:

    // 頂点バッファの作成処理
    std::vector<Vertex> createVertices() const override;

    // 描画処理
    void draw() const override;
};