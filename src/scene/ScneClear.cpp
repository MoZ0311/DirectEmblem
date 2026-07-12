// SceneClear class

# include "SceneClear.hpp"
# include "SceneManager.hpp"
# include "../core/Config.hpp"
# include "../util/InputState.hpp"

using namespace SceneSettings;

SceneClear::SceneClear()
	: backgroundRenderer{ Scene::Clear }
{

}

SceneClear::~SceneClear()
{

}

void SceneClear::update()
{
	if (InputState::KeyDown(VK_LBUTTON))
	{
		SceneManager::GetInstance().changeScene(Scene::Title);
		return;
	}
}

void SceneClear::draw() const
{
	backgroundRenderer.draw();
}