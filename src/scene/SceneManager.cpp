// SceneManager class

# include "SceneManager.hpp"

using namespace SceneSettings;

SceneManager::SceneManager()
	: m_currentScene{ nullptr }
	, m_transitionState{ TransitionState::None }
{

}

SceneManager& SceneManager::GetInstance()
{
	// 静的インスタンスを保持し、返す
	static SceneManager instance;
	return instance;
}

bool SceneManager::initialize(const Scene initScene)
{
	switch (initScene)
	{
	case SceneSettings::Scene::Title:
		// タイトルシーンのインスタンスを作成
		m_currentScene = std::make_unique<TitleScene>();
		break;

	case SceneSettings::Scene::Game:
		// ゲームシーンのインスタンスを作成
		m_currentScene = std::make_unique<GameScene>();
		break;

	default:
		// 不正なシーン引数が渡された時、falseを返す
		return false;
	}

	// 正常にシーンインスタンスが作られた時、true
	return true;
}

void SceneManager::execute()
{
	m_currentScene.get()->updateScene();
	m_currentScene.get()->drawScene();
}

void SceneManager::changeScene(const Scene targetScene)
{
	m_currentScene.reset();
	switch (targetScene)
	{
	case Scene::Title:
		m_currentScene = std::make_unique<TitleScene>();
		break;
	case Scene::Game:
		m_currentScene = std::make_unique<GameScene>();
		break;
	default:
		break;
	}
}