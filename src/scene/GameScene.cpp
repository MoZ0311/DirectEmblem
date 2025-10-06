// GameScene class

# include "GameScene.hpp"

GameScene::GameScene(const ComPtr<ID3D11Device>& device)
	: BaseScene(device)
{
	// îwåiêFÇê›íË
	m_backgroundColor = { 0.0f, 0.502f, 0.502f, 1.0f };
}

GameScene::~GameScene()
{

}

HRESULT GameScene::createVertexBuffer(const ComPtr<ID3D11Device>& device)
{
	return 0;
}

void GameScene::update()
{

}

void GameScene::draw() const
{

}

void GameScene::exit()
{

}