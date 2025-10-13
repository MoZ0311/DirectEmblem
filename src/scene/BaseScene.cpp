// BaseScene class

# include "BaseScene.hpp"

using namespace std::chrono;

BaseScene::BaseScene()
	: m_direct3D{ Direct3D::GetInstance() }
	, m_backgroundColor{ 0.0f, 0.0f, 0.0f, 0.0f }
	, m_prevTime{ high_resolution_clock::now() }
	, m_deltaTime{ 0.001f }
{

}

void BaseScene::calculateDeltaTime()
{
	const time_point<high_resolution_clock> currentTime{ high_resolution_clock::now() };

	const duration<float> deltaTime{ currentTime - m_prevTime };

	// 現在の時間を次のループの基準時間に更新
	m_prevTime = currentTime;

	// deltaTime更新
	m_deltaTime = deltaTime.count();
}

void BaseScene::updateScene()
{
	// deltaTimeの計算処理
	calculateDeltaTime();

	// 純粋仮想関数の更新処理実行
	update();	
}

void BaseScene::drawScene() const
{
	// シーンの画面をクリア
	m_direct3D.clearBackground(m_backgroundColor);

	// 純粋仮想関数の描画処理実行
	draw();

	// DirectX側でフリップ処理
	m_direct3D.flip();
}

float BaseScene::getDeltaTime() const
{
	return m_deltaTime;
}