// UnitManager class

# include "UnitManager.hpp"

# include "../core/Config.hpp"
# include "../scene/SceneManager.hpp"
# include "../unit/UnitSword.hpp"
# include "../unit/UnitAxe.hpp"
# include "../unit/UnitEnemy.hpp"

using namespace Config::MapSettings;
using namespace Config::UISettings;
using namespace Config::UnitSettings;

UnitManager::UnitManager()
	: m_playerUnitArray{}
	, m_enemyUnitArray{}
{
	initialize();
}

void UnitManager::initialize()
{
	m_playerUnitArray.push_back(std::make_unique<UnitSword>(GridPosition{10, 8}));
	m_playerUnitArray.push_back(std::make_unique<UnitAxe>(GridPosition{12, 9}));

	m_enemyUnitArray.push_back(std::make_unique<UnitEnemy>(GridPosition{ 0, 3 }));
	m_enemyUnitArray.push_back(std::make_unique<UnitEnemy>(GridPosition{ 1, 3 }));
	m_enemyUnitArray.push_back(std::make_unique<UnitEnemy>(GridPosition{ 2, 3 }));
	m_enemyUnitArray.push_back(std::make_unique<UnitEnemy>(GridPosition{ 3, 3 }));
}

UnitManager& UnitManager::GetInstance()
{
	// 静的インスタンスを保持し、返す
	static UnitManager instance;
	return instance;
}

void UnitManager::update()
{
	if (m_playerUnitArray.empty())
	{
		return;
	}
	if (m_enemyUnitArray.empty())
	{
		return;
	}

	for (const auto& playerUnit : m_playerUnitArray)
	{
		// 自軍ユニットの更新
		playerUnit->update();
	}

	for (const auto& enemyUnit : m_enemyUnitArray)
	{
		// 敵軍ユニットの更新
		enemyUnit->update();
	}
}

void UnitManager::draw() const
{
	for (const auto& playerUnit : m_playerUnitArray)
	{
		// 自軍ユニットの描画
		playerUnit->draw();
	}

	for (const auto& enemyUnit : m_enemyUnitArray)
	{
		// 敵軍ユニットの描画
		enemyUnit->draw();
	}
}

std::vector<std::vector<bool>> UnitManager::getUnitStandingGrid() const
{
	// ユニットの存在配列を宣言
	std::vector<std::vector<bool>> unitStandingGrid{};

	// 誰もいない(false)で初期化
	unitStandingGrid.assign(MapHeight, std::vector<bool>(MapWidth, false));

	// 範囲for文で自軍ユニットの座標を取得
	for (const auto& playerUnit : m_playerUnitArray)
	{
		// 自軍ユニットの位置を取得
		const GridPosition unitPosition{ playerUnit->getUnitPosition() };

		// ユニットの立ち位置更新
		unitStandingGrid[unitPosition.y][unitPosition.x] = true;
	}

	// 範囲for文で敵軍ユニットの座標を取得
	for (const auto& enemyUnit : m_enemyUnitArray)
	{
		// 敵軍ユニットの位置を取得
		const GridPosition unitPosition{ enemyUnit->getUnitPosition() };

		// ユニットの立ち位置更新
		unitStandingGrid[unitPosition.y][unitPosition.x] = true;
	}

	return unitStandingGrid;
}

const std::vector<std::unique_ptr<UnitBase>>& UnitManager::getPlayerUnitArray() const
{
	return m_playerUnitArray;
}

const std::vector<std::unique_ptr<UnitBase>>& UnitManager::getEnemyUnitArray() const
{
	return m_enemyUnitArray;
}

void UnitManager::resetAllUnitState()
{
	// 範囲for文で自軍ユニットの状態を初期化
	for (const auto& playerUnit : m_playerUnitArray)
	{
		playerUnit->unitState = UnitState::None;
	}

	// 範囲for文で敵軍ユニットの状態を初期化
	for (const auto& enemyUnit : m_enemyUnitArray)
	{
		enemyUnit->unitState = UnitState::None;
	}
}

void UnitManager::removeUnitPosition(const Config::MapSettings::GridPosition& position)
{
	// 自軍ユニット配列から削除
	auto player_it = std::remove_if(
		m_playerUnitArray.begin(),
		m_playerUnitArray.end(),
		[&position](const std::unique_ptr<UnitBase>& unit)
		{
			return unit->getUnitPosition() == position;
		});
	m_playerUnitArray.erase(player_it, m_playerUnitArray.end());


	// 敵軍ユニット配列から削除
	auto enemy_it = std::remove_if(
		m_enemyUnitArray.begin(),
		m_enemyUnitArray.end(),
		[&position](const std::unique_ptr<UnitBase>& unit)
		{
			return unit->getUnitPosition() == position;
		});
	m_enemyUnitArray.erase(enemy_it, m_enemyUnitArray.end());
}