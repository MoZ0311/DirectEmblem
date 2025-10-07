// BaseScene class

# include "BaseScene.hpp"

BaseScene::BaseScene()
	: m_direct3D{ Direct3D::GetInstance() }
	, m_backgroundColor{ 0.0f }
	, m_vertexBuffer{ nullptr }
	, m_vertexCount{ 0 }
{

}

bool BaseScene::initialize()
{
	if (!createVertexBuffer())
	{
		// 失敗時、エラーメッセージとともにreturn
		MessageBox(NULL, L"Scene: 頂点バッファの作成に失敗しました", L"エラー", MB_ICONERROR);
		return false;
	}

	// 全てが正常終了時、trueを返す
	return true;
}

void BaseScene::drawScene() const
{
	// シーンの画面をクリア
	m_direct3D.clearBackground(m_backgroundColor);

	m_direct3D.setVertexBuffer(m_vertexBuffer);

	// 純粋仮想関数の描画処理実行
	draw();

	// DirectX側で描画コマンドを実行
	m_direct3D.draw(m_vertexCount);

	// DirectX側でフリップ処理
	m_direct3D.flip();
}