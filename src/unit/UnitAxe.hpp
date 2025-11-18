// UnitAxe class

#pragma once

# include "UnitBase.hpp"

class UnitAxe : public UnitBase
{
public:

	// コンストラクタ
	UnitAxe(const Config::MapSettings::GridPosition& spawnPosition);

private:
	void setParameter(const Config::MapSettings::GridPosition& spawnPosition) override;
};