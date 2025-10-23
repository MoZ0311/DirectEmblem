// GameScene class

# include "GameScene.hpp"

using namespace SceneSettings;

GameScene::GameScene()
	: m_mapRenderer{ FieldMap::GetInstance() }
	, m_unitManager{}
{
	// 背景色を設定
	m_backgroundColor = { 0.961f, 0.871f, 0.702f, 1.0f };
}

GameScene::~GameScene()
{

}

void GameScene::update()
{
	m_mapRenderer.update();
	m_unitManager.update();

	// 他の全ての更新処理の後、シーン遷移の判定
	if (InputState::KeyPressed(VK_RBUTTON))
	{
		// SceneManager::GetInstance().changeScene(Scene::Title);
	}
}

void GameScene::draw() const
{
	m_mapRenderer.draw();
	m_unitManager.draw();
}