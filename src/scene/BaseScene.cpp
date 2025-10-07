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
	// 頂点情報の作成
	const std::vector<Vertex> vertices{ createVertices() };

	// 頂点数の設定
	m_vertexCount = static_cast<UINT>(vertices.size());

	// バッファの作成
	m_vertexBuffer = m_direct3D.createVertexBuffer(vertices);

	// バッファ作成の成否をreturn
	return m_vertexBuffer != nullptr;
}

void BaseScene::drawScene() const
{
	// シーンの画面をクリア
	m_direct3D.clearBackground(m_backgroundColor);

	m_direct3D.setVertexBuffer(m_vertexBuffer);

	// 純粋仮想関数の描画処理実行
	draw();

	// DirectX側でフリップ処理
	m_direct3D.flip();
}