// UIManager class

#pragma once

# include "../dx11/DirectX.hpp"
# include "../dx11/Texture.hpp"
# include "../core/Config.hpp"

class Direct3D;

class UIManager
{
public:

	// シングルトンインスタンスの生成/取得
	static UIManager& GetInstance();

	// リセット処理
	void resetState();

	// 更新処理
	void update();

	// 描画処理
	void draw() const;

	// コマンドUIの描画フラグ
	bool isDrawingCommandUI;

private:

	// コンストラクタ
	UIManager();

	// コピーコンストラクタを削除
	UIManager(const UIManager&) = delete;

	// 初期化処理
	void initialize();

	// タイル情報UI用頂点情報の作成処理
	std::vector<Util::Vertex> createTileUIVertices() const;

	// コマンドUI用頂点情報の作成処理
	std::vector<Util::Vertex> createCommandUIVertices() const;

	// ハイライト用の頂点情報の作成処理
	std::vector<Util::Vertex> createHighlightVertices() const;

	// Direct3Dクラスのインスタンス
	Direct3D& m_direct3D;

	// タイルUIのテクスチャ
	Texture m_grassUITexture;	// 草原
	Texture m_forestUITexture;	// 森
	Texture m_fenceUITexture;	// 柵
	Texture m_waterUITexture;	// 水

	// タイルUIの頂点数
	UINT m_tileUIVertexCount;

	// タイルUIの頂点バッファ
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_tileUIBuffer;

	// コマンドUIのテクスチャ
	Texture m_commandUITexture;

	// コマンドUIの頂点数
	UINT m_commandUIVertexCount;

	// コマンドUIの頂点バッファ
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_commandUIBuffer;

	// ハイライト用のテクスチャ
	Texture m_highlightTexture;

	// ハイライトの頂点数
	UINT m_highlightVertexCount;

	// ハイライトの頂点バッファ
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_highlightBuffer;

	// マウスがUI上にあるか
	bool m_mouseOnUI;

	// マウスオーバー中のコマンドのインデックス
	Config::UISettings::Command m_selectingCommand;
};