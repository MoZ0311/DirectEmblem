// Application class

# pragma once

# include <windows.h>

class Application
{
public:

	// コンストラクタ
	Application(const HINSTANCE& hInstance, const int nCmdShow);
	
	// デストラクタ
	~Application() = default;

	// 初期化処理
	bool initialize();

	// メインループ処理
	MSG run() const;

private:

	// ウィンドウプロシージャ(メッセージ処理)
	static LRESULT CALLBACK wndProc(const HWND hWnd, const UINT message, const WPARAM wParam, const LPARAM lParam);

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