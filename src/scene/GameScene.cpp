// GameScene class

# include "GameScene.hpp"

GameScene::GameScene()
	: m_mapRenderer{}
{
	// ”wŒiF‚ğİ’è
	m_backgroundColor = { 0.0f, 0.0f, 0.0f, 1.0f };
}

GameScene::~GameScene()
{

}

void GameScene::update()
{

}

void GameScene::draw() const
{
	m_mapRenderer.draw();
}