// SwordUnit class

#pragma once

# include "BaseUnit.hpp"

class SwordUnit : public BaseUnit
{
public:

	// コンストラクタ
	SwordUnit();

private:
	void setParameter() override;
};