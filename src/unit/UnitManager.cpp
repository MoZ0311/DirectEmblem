// UnitManager class

# include "UnitManager.hpp"

# include "../core/Config.hpp"
# include "../scene/SceneManager.hpp"
# include "../unit/UnitSword.hpp"
# include "../unit/UnitAxe.hpp"
# include "../unit/UnitBow.hpp"
# include "../unit/UnitEnemy.hpp"
# include "../unit/UnitEnemyBow.hpp"

using namespace Config::MapSettings;
using namespace Config::UISettings;
using namespace Config::UnitSettings;

UnitManager::UnitManager()
	: m_playerUnitArray{}
	, m_enemyUnitArray{}
{

}

void UnitManager::generateUnits()
{
	// 黒鷲: 主人公陣営
	{
		m_playerUnitArray.push_back(std::make_unique<UnitSword>(GridPosition{ 12, 14 }));
		m_playerUnitArray.push_back(std::make_unique<UnitSword>(GridPosition{ 13, 14 }));
		m_playerUnitArray.push_back(std::make_unique<UnitBow>(GridPosition{ 14, 14 }));
		m_playerUnitArray.push_back(std::make_unique<UnitBow>(GridPosition{ 15, 13 }));
		m_playerUnitArray.push_back(std::make_unique<UnitSword>(GridPosition{ 15, 12 }));
	}

	// 金鹿
	{
		m_enemyUnitArray.push_back(std::make_unique<UnitEnemy>(GridPosition{ 4, 3 }));
		m_enemyUnitArray.push_back(std::make_unique<UnitEnemy>(GridPosition{ 6, 8 }));
		m_enemyUnitArray.push_back(std::make_unique<UnitEnemyBow>(GridPosition{ 5, 8 }));
		m_enemyUnitArray.push_back(std::make_unique<UnitEnemyBow>(GridPosition{ 9, 10 }));
	}

	// 青獅子
	{
		m_enemyUnitArray.push_back(std::make_unique<UnitEnemyBow>(GridPosition{ 18, 1 }));
		m_enemyUnitArray.push_back(std::make_unique<UnitEnemy>(GridPosition{ 18, 3 }));
		m_enemyUnitArray.push_back(std::make_unique<UnitEnemyBow>(GridPosition{ 15, 1 }));
		m_enemyUnitArray.push_back(std::make_unique<UnitEnemy>(GridPosition{ 15, 5 }));
		m_enemyUnitArray.push_back(std::make_unique<UnitEnemyBow>(GridPosition{ 14, 8 }));
	}
}

UnitManager& UnitManager::GetInstance()
{
	// 静的インスタンスを保持し、返す
	static UnitManager instance;
	return instance;
}

void UnitManager::resetState()
{
	// 配列を全てクリア
	m_playerUnitArray.clear();
	m_enemyUnitArray.clear();
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

	m_playerUnitArray.erase(
		std::remove_if(m_playerUnitArray.begin(), m_playerUnitArray.end(),
			[](const std::unique_ptr<UnitBase>& u) { return u->isDead; }),
		m_playerUnitArray.end());

	m_enemyUnitArray.erase(
		std::remove_if(m_enemyUnitArray.begin(), m_enemyUnitArray.end(),
			[](const std::unique_ptr<UnitBase>& u) { return u->isDead; }),
		m_enemyUnitArray.end());
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
		if (playerUnit->isDead)
		{
			continue;
		}

		// 自軍ユニットの位置を取得
		const GridPosition unitPosition{ playerUnit->getUnitPosition() };

		// ユニットの立ち位置更新
		unitStandingGrid[unitPosition.y][unitPosition.x] = true;
	}

	// 範囲for文で敵軍ユニットの座標を取得
	for (const auto& enemyUnit : m_enemyUnitArray)
	{
		if (enemyUnit->isDead)
		{
			continue;
		}

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

Config::UnitSettings::UnitType UnitManager::getUnitTypeAtPosition(const Config::MapSettings::GridPosition& position) const
{
	// 範囲for文で自軍ユニットの座標を取得
	for (const auto& playerUnit : m_playerUnitArray)
	{
		if (playerUnit->isDead)
		{
			continue;
		}

		// 自軍ユニットの位置を取得
		const GridPosition unitPosition{ playerUnit->getUnitPosition() };

		// ターゲット座標と一致していれば、return
		if (unitPosition == position)
		{
			return playerUnit->getUnitType();
		}
	}

	// 範囲for文で敵軍ユニットの座標を取得
	for (const auto& enemyUnit : m_enemyUnitArray)
	{
		if (enemyUnit->isDead)
		{
			continue;
		}

		// 敵軍ユニットの位置を取得
		const GridPosition unitPosition{ enemyUnit->getUnitPosition() };

		// ターゲット座標と一致していれば、return
		if (unitPosition == position)
		{
			return enemyUnit->getUnitType();
		}
	}

	return UnitType::None;
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

void UnitManager::removeUnitAtPosition(const Config::MapSettings::GridPosition& position)
{
	// 範囲for文で自軍ユニットの座標を取得
	for (const auto& playerUnit : m_playerUnitArray)
	{
		// 自軍ユニットの位置を取得
		const GridPosition unitPosition{ playerUnit->getUnitPosition() };

		// ターゲット座標と一致していれば、return
		if (unitPosition == position)
		{
			playerUnit->isDead = true;
			return;
		}
	}

	// 範囲for文で敵軍ユニットの座標を取得
	for (const auto& enemyUnit : m_enemyUnitArray)
	{
		// 敵軍ユニットの位置を取得
		const GridPosition unitPosition{ enemyUnit->getUnitPosition() };

		// ターゲット座標と一致していれば、return
		if (unitPosition == position)
		{
			enemyUnit->isDead = true;
			return;
		}
	}
}