// UnitManager class

# include "UnitManager.hpp"

using namespace Config::MapSettings;

UnitManager::UnitManager()
	: isUnitMoving{ false }
	, m_playerUnitArray{}
	, m_enemyUnitArray{}
{
	m_playerUnitArray.push_back(std::make_unique<SwordUnit>());
	m_playerUnitArray.push_back(std::make_unique<AxeUnit>());
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