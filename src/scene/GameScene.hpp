// GameScene class

# pragma once

# include "BaseScene.hpp"
# include "SceneManager.hpp"
# include "../map/MapRenderer.hpp"
# include "../unit/UnitManager.hpp"

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
    MapRenderer& m_mapRenderer;

    // ユニット管理クラス
    UnitManager m_unitManager;
};