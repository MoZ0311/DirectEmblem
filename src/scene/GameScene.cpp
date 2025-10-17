// GameScene class

# include "GameScene.hpp"

using namespace SceneSettings;

GameScene::GameScene()
	: m_mapRenderer{}
	, m_unitManager{}
{
	// ”wŒiF‚ğİ’è
	m_backgroundColor = { 0.961f, 0.871f, 0.702f, 1.0f };
}

GameScene::~GameScene()
{

}

void GameScene::update()
{
	if (InputState::KeyPressed(VK_RBUTTON))
	{
		SceneManager::GetInstance().changeScene(Scene::Title);
	}

	m_unitManager.update();
}

void GameScene::draw() const
{
	m_mapRenderer.draw();
	m_unitManager.draw();
}