// TitleScene class

# include "TitleScene.hpp"

using namespace SceneSettings;

TitleScene::TitleScene()
	: backgroundRenderer{}
{
	// îwåiêFÇê›íË
	m_backgroundColor = { 0.0f, 0.0f, 0.251f, 1.0f };
}

TitleScene::~TitleScene()
{

}

void TitleScene::update()
{
	if (InputState::KeyPressed(VK_SPACE))
	{
		SceneManager::GetInstance().changeScene(Scene::Game);
	}
}

void TitleScene::draw() const
{
	backgroundRenderer.draw();
}