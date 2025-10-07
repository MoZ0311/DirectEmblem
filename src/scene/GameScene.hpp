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
    bool createVertexBuffer() override;

    // 描画処理
    void draw() const override;
};