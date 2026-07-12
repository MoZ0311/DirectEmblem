// UnitEnemyBow class

#pragma once

# include "UnitBase.hpp"

class UnitEnemyBow : public UnitBase
{
public:

	// コンストラクタ
	UnitEnemyBow(const Config::MapSettings::GridPosition& spawnPosition);

private:
	void setParameter(const Config::MapSettings::GridPosition& spawnPosition) override;

	// 敵ユニットAI: ターゲットを決定し、移動先/攻撃を判断する
	void decideAction() override;
};