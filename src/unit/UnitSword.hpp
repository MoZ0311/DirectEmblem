// UnitSword class

#pragma once

# include "UnitBase.hpp"

class UnitSword : public UnitBase
{
public:

	// コンストラクタ
	UnitSword(const Config::MapSettings::GridPosition& spawnPosition);

private:
	void setParameter(const Config::MapSettings::GridPosition& spawnPosition) override;
};