// BaseScene class

# include "BaseScene.hpp"

BaseScene::BaseScene(ComPtr<ID3D11Device> device)
	: m_backgroundColor{ 0.0f }
	, m_vertexBuffer{ nullptr }
	, m_vertexCount{ 0 }
{
	initialize(device);
}

bool BaseScene::initialize(ComPtr<ID3D11Device> device)
{
	const HRESULT vertexBufferResult{ createVertexBuffer(device) };
	if (FAILED(vertexBufferResult))
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
	// 純粋仮想関数の描画処理実行
	draw();

	// シーンの描画情報をDirectXに転送
	Direct3D::GetInstance().draw(m_vertexCount, m_backgroundColor);
}