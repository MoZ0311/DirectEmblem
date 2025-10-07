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

bool TitleScene::createVertexBuffer()
{
	// 画面中央に配置する一つの正方形の頂点データ
	std::vector<Vertex> vertices{
		// 頂点データ: { x, y, z, r, g, b, a } 
		{ -0.9f,  0.9f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f }, // 左上 (赤)
		{  0.9f,  0.9f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f }, // 右上 (緑)
		{ -0.9f, -0.9f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f }, // 左下 (青)
		{  0.9f, -0.9f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f }, // 右下 (黄)
	};

	// 頂点数の設定
	m_vertexCount = static_cast<UINT>(vertices.size());

	// バッファの作成
	m_vertexBuffer = m_direct3D.createVertexBuffer(vertices);

	// バッファ作成の成否をreturn
	return m_vertexBuffer != nullptr;
}

void TitleScene::updateScene()
{

}

void TitleScene::draw() const
{
	
}