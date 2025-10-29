// SceneManager class

# pragma once

# include <chrono>
# include "TitleScene.hpp"
# include "GameScene.hpp"

class SceneManager
{
public:

	// シングルトンインスタンスの作成/取得
	static SceneManager& GetInstance();

	// 初期化処理
	bool initialize(const SceneSettings::Scene initScene);

	// ゲーム処理の更新/実行
	void execute();

	// シーンチェンジ
	void changeScene(const SceneSettings::Scene targetScene);

	// deltaTimeの取得処理
	float getDeltaTime() const;

private:

	// コンストラクタ
	SceneManager();

	// deltaTimeの計算処理
	void calculateDeltaTime();

	// 現在のシーン
	std::unique_ptr<BaseScene> m_currentScene;

	// シーンの切替状態
	SceneSettings::TransitionState m_transitionState;

	// 直前の時間
	std::chrono::time_point<std::chrono::high_resolution_clock> m_prevTime;

	// 前フレームからの経過時間
	float m_deltaTime;
};