// AxeUnit class

# include "AxeUnit.hpp"

using namespace Config::UnitSettings;

AxeUnit::AxeUnit()
{
	setParameter();
}

void AxeUnit::setParameter()
{
	setPosition({ 12, 8 });
	m_unitType = UnitType::Axe;
	m_unitIconTexture = Texture{ L"assets/images/icon_sample.png" };

	m_unitParameter.mobility = 5;
}