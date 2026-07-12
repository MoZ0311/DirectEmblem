// SceneGame class

# include "SceneGame.hpp"

# include "../map/FieldMap.hpp"
# include "../unit/UnitManager.hpp"
# include "../battle/BattleManager.hpp"
# include "../scene/SceneManager.hpp"
# include "../ui/UIManager.hpp"
# include "../util/InputState.hpp"

using namespace SceneSettings;

SceneGame::SceneGame()
	: m_fieldMap{ FieldMap::GetInstance() }
	, m_unitManager{ UnitManager::GetInstance() }
	, m_battleManager{ BattleManager::GetInstance() }
	, m_uiManager{ UIManager::GetInstance() }
{
	// 背景色を設定
	m_backgroundColor = { 0.961f, 0.871f, 0.702f, 1.0f };

	// ユニットの初期配置
	m_unitManager.generateUnits();
}

SceneGame::~SceneGame()
{
	m_fieldMap.resetState();
	m_unitManager.resetState();
	m_battleManager.resetState();
	m_uiManager.resetState();
}

void SceneGame::update()
{
	m_fieldMap.update();
	m_unitManager.update();
	m_battleManager.update();
	m_uiManager.update();

	// 他の全ての更新処理の後、シーン遷移の判定

	// 自軍ユニットの全滅
	if (m_unitManager.getPlayerUnitArray().empty())
	{
		SceneManager::GetInstance().changeScene(Scene::Over);
		return;
	}

	// 敵軍ユニットの全滅
	if (m_unitManager.getEnemyUnitArray().empty())
	{
		SceneManager::GetInstance().changeScene(Scene::Clear);
		return;
	}
}

void SceneGame::draw() const
{
	m_fieldMap.draw();
	m_unitManager.draw();
	m_uiManager.draw();
}