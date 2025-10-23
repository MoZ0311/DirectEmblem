// GameScene class

# pragma once

# include "BaseScene.hpp"
# include "SceneManager.hpp"
# include "../map/FieldMap.hpp"
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

    // FieldMapクラスのインスタンス
    FieldMap& m_mapRenderer;

    // ユニット管理クラス
    UnitManager m_unitManager;
};