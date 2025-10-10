// TitleScene class

# include "TitleScene.hpp"

TitleScene::TitleScene()
{
	// 背景色を設定
	m_backgroundColor = { 0.0f, 0.0f, 0.251f, 1.0f };
}

TitleScene::~TitleScene()
{

}

void TitleScene::update()
{
	
}

void TitleScene::draw() const
{
	// DirectXにTitleSceneのバッファを転送
	// m_direct3D.setVertexBuffer(m_vertexBuffer);

	// DirectX側で描画コマンドを実行
	// m_direct3D.draw(m_vertexCount);
}