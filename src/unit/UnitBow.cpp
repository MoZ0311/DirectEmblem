// UnitBow class

# include "UnitBow.hpp"

using namespace Config::UnitSettings;
using namespace FilePath;

UnitBow::UnitBow(const Config::MapSettings::GridPosition& spawnPosition)
{
	setParameter(spawnPosition);
}

void UnitBow::setParameter(const Config::MapSettings::GridPosition& spawnPosition)
{
	setPosition(spawnPosition);
	m_unitType = UnitType::Bow;
	m_unitIconTexture = Texture{ PlayerArcherIconPath };

	m_unitParameter.mobility = 3;
	m_unitParameter.attackRange = 2;
}