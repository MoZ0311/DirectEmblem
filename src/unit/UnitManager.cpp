// UnitManager class

# include "UnitManager.hpp"

using namespace Config::MapSettings;

UnitManager::UnitManager()
	: isUnitMoving{ false }
	, m_playerUnitArray{}
	, m_enemyUnitArray{}
	, m_unitStandingGrid{}
{
	// ユニットの存在配列の初期化
	m_unitStandingGrid.assign(MapHeight, std::vector<bool>(MapWidth, false));

	m_playerUnitArray.push_back(std::make_unique<SwordUnit>());
}

UnitManager& UnitManager::GetInstance()
{
	// 静的インスタンスを保持し、返す
	static UnitManager instance;
	return instance;
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

std::vector<std::vector<bool>> UnitManager::getUnitStandingGrid() const
{
	return m_unitStandingGrid;
}