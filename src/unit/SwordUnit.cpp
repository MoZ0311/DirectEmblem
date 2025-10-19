// SwordUnit class

# include "SwordUnit.hpp"

using namespace Config::UnitSettings;

SwordUnit::SwordUnit()
{
	setParameter();
}

void SwordUnit::setParameter()
{
	m_unitType = UnitType::Sword;
	m_unitIconTexture = Texture{ L"assets/images/icon_sample.png" };
}