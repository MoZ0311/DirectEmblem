// UnitSword class

# include "UnitSword.hpp"

using namespace Config::UnitSettings;

UnitSword::UnitSword(const Config::MapSettings::GridPosition& spawnPosition)
{
	setParameter(spawnPosition);
}

void UnitSword::setParameter(const Config::MapSettings::GridPosition& spawnPosition)
{
	setPosition(spawnPosition);
	m_unitType = UnitType::Sword;
	m_unitIconTexture = Texture{ L"assets/images/icon_sample.png" };

	m_unitParameter.mobility = 4;
}