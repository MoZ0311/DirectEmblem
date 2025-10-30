// AxeUnit class

#pragma once

# include "BaseUnit.hpp"

class AxeUnit : public BaseUnit
{
public:

	// コンストラクタ
	AxeUnit();

private:
	void setParameter() override;
};