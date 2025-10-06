// Application class

# pragma once

# include "Config.hpp"
# include "../scene/SceneManager.hpp"
# include "../dx11/Direct3D.hpp"

class Application
{
public:

	// コンストラクタ
	Application(HINSTANCE hInstance, int nCmdShow);

	// 初期化処理
	bool initialize();

	// メインループ処理
	MSG run() const;

private:

	// ウィンドウプロシージャ(メッセージ処理)
	static LRESULT CALLBACK wndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	// ウィンドウクラスの登録
	bool registerWindowClass() const;

	// ウィンドウの作成
	bool createWindow();

	// アプリケーションのハンドル
	HINSTANCE m_hInstance;

	// ウィンドウの表示方法
	int m_nCmdShow;

	// ウィンドウの識別ハンドル
	HWND m_hWnd;
};