// GameScene class

# pragma once

# include "BaseScene.hpp"
# include "../map/MapRenderer.hpp"

class GameScene : public BaseScene
{
public:

    // コンストラクタ
    GameScene();

    // デストラクタ
    ~GameScene();

private:

    // 更新処理
    void update() override;

    // 描画処理
    void draw() const override;

    // マップ描画クラス
    MapRenderer m_mapRenderer;
};