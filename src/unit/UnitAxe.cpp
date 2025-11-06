// UnitAxe class

# include "UnitAxe.hpp"

using namespace Config::UnitSettings;

UnitAxe::UnitAxe()
{
	setParameter();
}

void UnitAxe::setParameter()
{
	setPosition({ 12, 8 });
	m_unitType = UnitType::Axe;
	m_unitIconTexture = Texture{ L"assets/images/icon_sample.png" };

	m_unitParameter.mobility = 5;
}