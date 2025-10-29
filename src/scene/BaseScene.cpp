// BaseScene class

# include "BaseScene.hpp"

BaseScene::BaseScene()
	: m_direct3D{ Direct3D::GetInstance() }
	, m_backgroundColor{ 0.0f, 0.0f, 0.0f, 0.0f }
{

}

void BaseScene::updateScene()
{
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