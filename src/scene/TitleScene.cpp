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

std::vector<Vertex> TitleScene::createVertices() const
{
	// 画面中央に配置する一つの正方形の頂点データ
	const std::vector<Vertex> vertices{
		// 頂点データ: { x, y, z, r, g, b, a } 
		{ -0.9f,  0.9f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f }, // 左上 (赤)
		{  0.9f,  0.9f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f }, // 右上 (緑)
		{ -0.9f, -0.9f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f }, // 左下 (青)
		{  0.9f, -0.9f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f }, // 右下 (黄)
	};

	return vertices;
}

void TitleScene::updateScene()
{

}

void TitleScene::draw() const
{
	// DirectX側で描画コマンドを実行
	m_direct3D.draw(m_vertexCount);
}