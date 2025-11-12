// UIManager class

#pragma once

# include "../dx11/DirectX.hpp"
# include "../dx11/Texture.hpp"
# include "../core/Config.hpp"

class Direct3D;
class UnitBase;

class UIManager
{
public:

	// シングルトンインスタンスの生成/取得
	static UIManager& GetInstance();

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

	// UI用頂点情報の作成処理
	std::vector<Util::Vertex> createCommandUIVertices() const;

	// ハイライト用の頂点情報の作成処理
	std::vector<Util::Vertex> createHighlightVertices() const;

	// Direct3Dクラスのインスタンス
	Direct3D& m_direct3D;

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