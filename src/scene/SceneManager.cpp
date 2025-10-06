// SceneManager class

# include "SceneManager.hpp"

using namespace SceneSettings;

SceneManager::SceneManager()
	: m_device{ nullptr }
	, m_currentScene{ nullptr }
	, m_transitionState{ TransitionState::None }
{
	int i{ 50 };
}

SceneManager& SceneManager::GetInstance()
{
	// 静的インスタンスを保持し、返す
	static SceneManager instance;
	return instance;
}

bool SceneManager::initialize(ComPtr<ID3D11Device> device, Scene initScene)
{
	m_device = device;

	switch (initScene)
	{
	case SceneSettings::Scene::Title:
		// タイトルシーンのインスタンスを作成してreturn
		m_currentScene = std::make_unique<TitleScene>(m_device);
		return true;
		break;

	case SceneSettings::Scene::Game:
		// ゲームシーンのインスタンスを作成してreturn
		m_currentScene = std::make_unique<GameScene>(m_device);
		return true;
		break;

	default:
		break;
	}

	return false;
}

void SceneManager::execute()
{
	m_currentScene.get()->update();
	m_currentScene.get()->drawScene();
}

void SceneManager::changeScene(Scene targetScene)
{
	m_currentScene.reset();
	switch (targetScene)
	{
	case Scene::Title:
		m_currentScene = std::make_unique<TitleScene>(m_device);
		break;
	case Scene::Game:
		m_currentScene = std::make_unique<GameScene>(m_device);
		break;
	default:
		break;
	}
}