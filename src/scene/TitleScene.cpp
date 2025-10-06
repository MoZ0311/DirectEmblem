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

HRESULT TitleScene::createVertexBuffer(const ComPtr<ID3D11Device>& device)
{
	// 画面中央に配置する一つの正方形の頂点データ
	Vertex vertices[]{
		// 頂点データ: { x, y, z, r, g, b, a } 
		{ -0.9f,  0.9f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f }, // 左上 (赤)
		{  0.9f,  0.9f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f }, // 右上 (緑)
		{ -0.9f, -0.9f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f }, // 左下 (青)
		{  0.9f, -0.9f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f }, // 右下 (黄)
	};

	// 頂点数の設定
	m_vertexCount = ARRAYSIZE(vertices);

	// 頂点バッファの設定
	D3D11_BUFFER_DESC bufferDesc{};
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(vertices);
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;

	// サブリソースデータ（初期データ）の設定
	D3D11_SUBRESOURCE_DATA initData{};
	initData.pSysMem = vertices;

	// 頂点バッファの作成と結果の返却
	return device->CreateBuffer(
		&bufferDesc,
		&initData,
		m_vertexBuffer.GetAddressOf()
	);
}

void TitleScene::update()
{

}

void TitleScene::draw() const
{
	
}

void TitleScene::exit()
{

}