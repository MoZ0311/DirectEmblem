// SceneGame class

# include "SceneGame.hpp"

# include "../map/FieldMap.hpp"
# include "../unit/UnitManager.hpp"
# include "../ui/UIManager.hpp"
# include "../util/InputState.hpp"

using namespace SceneSettings;

SceneGame::SceneGame()
	: m_mapRenderer{ FieldMap::GetInstance() }
	, m_unitManager{ UnitManager::GetInstance() }
	, m_uiManager{ UIManager::GetInstance() }
{
	// 背景色を設定
	m_backgroundColor = { 0.961f, 0.871f, 0.702f, 1.0f };
}

SceneGame::~SceneGame()
{

}

void SceneGame::update()
{
	m_mapRenderer.update();
	m_unitManager.update();

	// 他の全ての更新処理の後、シーン遷移の判定
	if (InputState::KeyPressed(VK_RBUTTON))
	{
		// SceneManager::GetInstance().changeScene(Scene::Title);
	}
}

void SceneGame::draw() const
{
	m_mapRenderer.draw();
	m_unitManager.draw();
	m_uiManager.draw();
}