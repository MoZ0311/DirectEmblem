// UnitAxe class

# include "UnitAxe.hpp"

using namespace Config::UnitSettings;

UnitAxe::UnitAxe(const Config::MapSettings::GridPosition& spawnPosition)
{
	setParameter(spawnPosition);
}

void UnitAxe::setParameter(const Config::MapSettings::GridPosition& spawnPosition)
{
	setPosition(spawnPosition);
	m_unitType = UnitType::Axe;
	m_unitIconTexture = Texture{ L"assets/images/icon_sample.png" };

	m_unitParameter.mobility = 5;
}