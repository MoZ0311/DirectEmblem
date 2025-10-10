// GameScene class

# include "GameScene.hpp"

GameScene::GameScene()
	: m_mapRenderer{}
{
	// 背景色を設定
	m_backgroundColor = { 0.0f, 0.502f, 0.502f, 1.0f };
}

GameScene::~GameScene()
{

}

void GameScene::update()
{

}

void GameScene::draw() const
{
	// DirectXにTitleSceneのバッファを転送
	//m_direct3D.setVertexBuffer(m_vertexBuffer);
	//m_direct3D.draw(m_vertexCount);

	m_mapRenderer.draw();
}