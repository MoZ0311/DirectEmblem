// UnitAxe class

#pragma once

# include "UnitBase.hpp"

class UnitAxe : public UnitBase
{
public:

	// コンストラクタ
	UnitAxe();

private:
	void setParameter() override;
};