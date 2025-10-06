// SceneManager class

# pragma once

# include "../core/Config.hpp"
# include "../dx11/Direct3D.hpp"
# include "TitleScene.hpp"
# include "GameScene.hpp"

class SceneManager
{
public:

	// シングルトンインスタンスの作成/取得
	static SceneManager& GetInstance();

	// 初期化処理
	bool initialize(const ComPtr<ID3D11Device>& device, const SceneSettings::Scene initScene);

	// ゲーム処理の更新/実行
	void execute();

	// シーンチェンジ
	void changeScene(const SceneSettings::Scene targetScene);

private:

	// コンストラクタ
	SceneManager();

	// Direct3Dのデバイス
	ComPtr<ID3D11Device> m_device;

	// 現在のシーン
	std::unique_ptr<BaseScene> m_currentScene;

	// シーンの切替状態
	SceneSettings::TransitionState m_transitionState;
};