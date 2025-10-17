// UnitManager class

#pragma once

# include "UnitBase.hpp"

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
	std::vector<std::unique_ptr<UnitBase>> m_playerUnitArray;

	// 敵軍ユニット配列
	std::vector<std::unique_ptr<UnitBase>> m_enemyUnitArray;
};