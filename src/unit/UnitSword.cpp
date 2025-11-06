// UnitSword class

# include "UnitSword.hpp"

using namespace Config::UnitSettings;

UnitSword::UnitSword()
{
	setParameter();
}

void UnitSword::setParameter()
{
	setPosition({ 10, 8 });
	m_unitType = UnitType::Sword;
	m_unitIconTexture = Texture{ L"assets/images/icon_sample.png" };

	m_unitParameter.mobility = 4;
}