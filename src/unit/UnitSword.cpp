// UnitSword class

# include "UnitSword.hpp"

using namespace Config::UnitSettings;
using namespace FilePath;

UnitSword::UnitSword(const Config::MapSettings::GridPosition& spawnPosition)
{
	setParameter(spawnPosition);
}

void UnitSword::setParameter(const Config::MapSettings::GridPosition& spawnPosition)
{
	setPosition(spawnPosition);
	m_unitType = UnitType::Sword;
	m_unitIconTexture = Texture{ PlayerIconPath };

	m_unitParameter.mobility = 5;
}