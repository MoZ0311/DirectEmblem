// UnitManager class

#pragma once

# include "SwordUnit.hpp"

class UnitManager
{
public:

	// シングルトンインスタンスの生成/取得
	static UnitManager& GetInstance();

	// 更新処理
	void update();

	// 描画処理
	void draw() const;

	// ユニットの存在二次元配列の取得
	std::vector<std::vector<bool>> getUnitStandingGrid() const;

	// 任意のユニットを選択中であるか
	bool isUnitMoving;

private:

	// コンストラクタ
	UnitManager();

	// コピーコンストラクタを削除
	UnitManager(const UnitManager&) = delete;

	// 自軍ユニット配列
	std::vector<std::unique_ptr<BaseUnit>> m_playerUnitArray;

	// 敵軍ユニット配列
	std::vector<std::unique_ptr<BaseUnit>> m_enemyUnitArray;

	// マップ上にユニットがいるかどうかの二次元配列
	std::vector<std::vector<bool>> m_unitStandingGrid;
};