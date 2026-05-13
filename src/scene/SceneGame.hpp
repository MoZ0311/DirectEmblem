// SceneGame class

# pragma once

# include "SceneBase.hpp"

class FieldMap;
class UnitManager;
class BattleManager;
class UIManager;

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
    FieldMap& m_fieldMap;

    // ユニット管理クラス
    UnitManager& m_unitManager;

    // 戦闘管理クラス
    BattleManager& m_battleManager;

    // UI管理クラス
    UIManager& m_uiManager;
};