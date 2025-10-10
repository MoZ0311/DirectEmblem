// SceneManager class

# pragma once

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

private:

	// コンストラクタ
	SceneManager();

	// 現在のシーン
	std::unique_ptr<BaseScene> m_currentScene;

	// シーンの切替状態
	SceneSettings::TransitionState m_transitionState;
};