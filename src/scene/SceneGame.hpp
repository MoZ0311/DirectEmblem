// SceneGame class

# pragma once

# include "SceneBase.hpp"

class FieldMap;
class UnitManager;

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