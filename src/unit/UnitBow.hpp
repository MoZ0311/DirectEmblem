// UnitBow class

#pragma once

# include "UnitBase.hpp"

class UnitBow : public UnitBase
{
public:

	// コンストラクタ
	UnitBow(const Config::MapSettings::GridPosition& spawnPosition);

private:
	void setParameter(const Config::MapSettings::GridPosition& spawnPosition) override;
};