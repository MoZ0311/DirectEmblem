// SceneOver class

# include "SceneOver.hpp"
# include "SceneManager.hpp"
# include "../core/Config.hpp"
# include "../util/InputState.hpp"

using namespace SceneSettings;

SceneOver::SceneOver()
	: backgroundRenderer{ Scene::Over }
{

}

SceneOver::~SceneOver()
{

}

void SceneOver::update()
{
	if (InputState::KeyDown(VK_LBUTTON))
	{
		SceneManager::GetInstance().changeScene(Scene::Game);
		return;
	}
}

void SceneOver::draw() const
{
	backgroundRenderer.draw();
}