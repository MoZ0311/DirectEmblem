// SceneTitle class

# include "SceneTitle.hpp"

using namespace SceneSettings;

SceneTitle::SceneTitle()
	: backgroundRenderer{}
{

}

SceneTitle::~SceneTitle()
{

}

void SceneTitle::update()
{
	if (InputState::KeyPressed(VK_LBUTTON))
	{
		SceneManager::GetInstance().changeScene(Scene::Game);
	}
}

void SceneTitle::draw() const
{
	backgroundRenderer.draw();
}