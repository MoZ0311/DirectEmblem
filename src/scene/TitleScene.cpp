// TitleScene class

# include "TitleScene.hpp"

using namespace SceneSettings;

TitleScene::TitleScene()
	: backgroundRenderer{}
{

}

TitleScene::~TitleScene()
{

}

void TitleScene::update()
{
	if (InputState::KeyPressed(VK_LBUTTON))
	{
		SceneManager::GetInstance().changeScene(Scene::Game);
	}
}

void TitleScene::draw() const
{
	backgroundRenderer.draw();
}