// SceneManager class

# include "SceneManager.hpp"

# include "SceneTitle.hpp"
# include "SceneGame.hpp"
# include "SceneClear.hpp"
# include "SceneOver.hpp"

using namespace std::chrono;
using namespace SceneSettings;

SceneManager::SceneManager()
	: m_currentScene{ nullptr }
	, m_transitionState{ TransitionState::None }
	, m_prevTime{ high_resolution_clock::now() }
	, m_deltaTime{ 0.001f }
{

}

SceneManager& SceneManager::GetInstance()
{
	// 静的インスタンスを保持し、返す
	static SceneManager instance;
	return instance;
}

bool SceneManager::initialize(const Scene& initScene)
{
	switch (initScene)
	{
	case Scene::Title:
		// タイトルシーンのインスタンスを作成
		m_currentScene = std::make_unique<SceneTitle>();
		break;

	case Scene::Game:
		// ゲームシーンのインスタンスを作成
		m_currentScene = std::make_unique<SceneGame>();
		break;

	case Scene::Clear:
		// クリアシーンのインスタンスを作成
		m_currentScene = std::make_unique<SceneClear>();
		break;

	case Scene::Over:
		// ゲームオーバーシーンのインスタンスを作成
		m_currentScene = std::make_unique<SceneOver>();
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
	// deltaTimeの算出
	calculateDeltaTime();

	m_currentScene->updateScene();
	m_currentScene->drawScene();
}

void SceneManager::calculateDeltaTime()
{
	const time_point<high_resolution_clock> currentTime{ high_resolution_clock::now() };

	const duration<float> deltaTime{ currentTime - m_prevTime };

	// 現在の時間を次のループの基準時間に更新
	m_prevTime = currentTime;

	// deltaTime更新
	m_deltaTime = deltaTime.count();
}

void SceneManager::changeScene(const Scene& targetScene)
{
	m_currentScene.reset();
	switch (targetScene)
	{
	case Scene::Title:
		m_currentScene = std::make_unique<SceneTitle>();
		break;

	case Scene::Game:
		m_currentScene = std::make_unique<SceneGame>();
		break;

	case Scene::Clear:
		m_currentScene = std::make_unique<SceneClear>();
		break;

	case Scene::Over:
		m_currentScene = std::make_unique<SceneOver>();
		break;

	default:
		break;
	}
}

float SceneManager::getDeltaTime() const
{
	return m_deltaTime;
}