// UnitManager class

# include "UnitManager.hpp"

UnitManager::UnitManager()
	: m_playerUnitArray{}
	, m_enemyUnitArray{}
{
	m_playerUnitArray.push_back(std::make_unique<UnitBase>());
}

void UnitManager::update()
{
	for (const auto& playerUnit : m_playerUnitArray)
	{
		playerUnit->update();
	}

	for (const auto& enemyUnit : m_enemyUnitArray)
	{
		enemyUnit->update();
	}
}

void UnitManager::draw() const
{
	for (const auto& playerUnit : m_playerUnitArray)
	{
		playerUnit->draw();
	}

	for (const auto& enemyUnit : m_enemyUnitArray)
	{
		enemyUnit->draw();
	}
}