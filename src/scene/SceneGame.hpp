// SceneGame class

# pragma once

# include "SceneBase.hpp"
# include "SceneManager.hpp"
# include "../map/FieldMap.hpp"
# include "../unit/UnitManager.hpp"

class SceneGame : public SceneBase
{
public:

    // コンストラクタ
    SceneGame();

    // デストラクタ
    ~SceneGame();

private:

    // 更新処理
    void update() override;

    // 描画処理
    void draw() const override;

    // FieldMapクラスのインスタンス
    FieldMap& m_mapRenderer;

    // ユニット管理クラス
    UnitManager& m_unitManager;
};