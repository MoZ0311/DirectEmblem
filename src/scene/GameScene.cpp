// GameScene class

# include "GameScene.hpp"

using namespace SceneSettings;

GameScene::GameScene()
	: m_mapRenderer{}
{
	// îwåiêFÇê›íË
	m_backgroundColor = { 0.0f, 0.5f, 0.5f, 1.0f };
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
}

void GameScene::draw() const
{
	m_mapRenderer.draw();
}