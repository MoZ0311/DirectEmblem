// UnitEnemy class

#pragma once

# include "UnitBase.hpp"

class UnitEnemy : public UnitBase
{
public:

	// コンストラクタ
	UnitEnemy(const Config::MapSettings::GridPosition& spawnPosition);

private:
	void setParameter(const Config::MapSettings::GridPosition& spawnPosition) override;

	// 敵ユニットAI: ターゲットを決定し、移動先/攻撃を判断する
	void decideAction() override;
};