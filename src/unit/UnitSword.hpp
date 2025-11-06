// UnitSword class

#pragma once

# include "UnitBase.hpp"

class UnitSword : public UnitBase
{
public:

	// コンストラクタ
	UnitSword();

private:
	void setParameter() override;
};