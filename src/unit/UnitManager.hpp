// UnitManager class

#pragma once

# include "BaseUnit.hpp"

class UnitManager
{
public:

	// コンストラクタ
	UnitManager();

	// 更新処理
	void update();

	// 描画処理
	void draw() const;

private:

	// 自軍ユニット配列
	std::vector<std::unique_ptr<BaseUnit>> m_playerUnitArray;

	// 敵軍ユニット配列
	std::vector<std::unique_ptr<BaseUnit>> m_enemyUnitArray;
};