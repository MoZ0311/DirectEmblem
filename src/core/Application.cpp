// Application class

# include "Application.hpp"

using namespace Config;

Application::Application(const HINSTANCE& hInstance, const int nCmdShow)
	: m_hInstance{ hInstance }
	, m_nCmdShow{ nCmdShow }
	, m_hWnd{ nullptr }
{
	// Direct3D class生成
	Direct3D::GetInstance();

	// SceneManager class生成
	SceneManager::GetInstance();
}

bool Application::initialize()
{
	// ウィンドウクラスの登録
	if (!registerWindowClass())
	{
		// 失敗時、エラーメッセージとともにreturn
		MessageBox(NULL, L"Application: ウィンドウクラスの登録に失敗しました", L"エラー", MB_ICONERROR);
		return false;
	}

	// ウィンドウの作成
	if (!createWindow())
	{
		// 失敗時、エラーメッセージとともにreturn
		MessageBox(NULL, L"Application: ウィンドウの作成に失敗しました", L"エラー", MB_ICONERROR);
		return false;
	}

	// DirectXの初期化
	if (!Direct3D::GetInstance().initialize(m_hWnd))
	{
		// 失敗時、エラーメッセージとともにreturn
		MessageBox(NULL, L"Application: DirectXの初期化に失敗しました", L"エラー", MB_ICONERROR);
		return false;
	}

	// SceneManagerの初期化(ついでに初期シーンの設定)
	if (!SceneManager::GetInstance().initialize(SceneSettings::Scene::Title))
	{
		// 失敗時、エラーメッセージとともにreturn
		MessageBox(NULL, L"Application: SceneManagerクラスの初期化に失敗しました", L"エラー", MB_ICONERROR);
		return false;
	}

	// 全てが正常終了時、trueを返す
	return true;
}

MSG Application::run() const
{
	MSG msg{};
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg); // キーボード入力を処理
		DispatchMessage(&msg);  // WndProcへメッセージを送信

		if (msg.wParam == VK_ESCAPE)
		{
			// Escキーで終了処理
			PostQuitMessage(0);
		}

		// マウス情報の更新
		InputState::UpdateMouseState(m_hWnd);

		// キー情報の更新
		InputState::UpdateKeyState();

		// ループ処理
		SceneManager::GetInstance().execute();
	}
	return msg;
}

LRESULT CALLBACK Application::wndProc(const HWND hWnd, const UINT message, const WPARAM wParam, const LPARAM lParam)
{
	if (message == WM_DESTROY)
	{
		// ウィンドウが閉じられた時、終了
		PostQuitMessage(0);
		return 0;
	}

	// 基本はデフォルトの処理関数に任せる
	return DefWindowProc(hWnd, message, wParam, lParam);
}

bool Application::registerWindowClass() const
{
	// 新規ウィンドウクラス構造体を生成
	WNDCLASSEX windowClassEX{};

	windowClassEX.cbSize = sizeof(WNDCLASSEX);					// 自身のサイズを保持
	windowClassEX.style = CS_HREDRAW | CS_VREDRAW;				// HorizontalとVerticalのREDRAW、つまり縦横の変更時に描画をやり直す設定
	windowClassEX.lpfnWndProc = wndProc;						// 使用するメッセージ処理関数
	windowClassEX.cbClsExtra = 0;								// クラスの専用追加メモリ。今回は使わないので0
	windowClassEX.cbWndExtra = 0;								// ウィンドウの追加メモリ。今回は使わないので0
	windowClassEX.hInstance = m_hInstance;						// 登録先のアプリケーションを識別するためのハンドル。
	windowClassEX.hIcon = LoadIcon(NULL, IDI_APPLICATION);		// アイコンは標準的なもの
	windowClassEX.hCursor = LoadCursor(NULL, IDC_ARROW);		// カーソルも標準のもの
	windowClassEX.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);	// 背景色は白
	windowClassEX.lpszMenuName = NULL;							// メニューバーは不要
	windowClassEX.lpszClassName = ClassName;					// クラス名
	windowClassEX.hIconSm = LoadIcon(NULL, IDI_APPLICATION);	// 小アイコンも標準のもの

	return RegisterClassEx(&windowClassEX);
}

bool Application::createWindow()
{
	// クライアント領域の矩形
	RECT windowRect{ 0, 0, WindowWidth, WindowHeight };

	// メニュー等の設定
	const DWORD dwStyle{ WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX };

	// 目的のクライアント領域サイズから、ウィンドウ全体のサイズを計算
	AdjustWindowRect(&windowRect, dwStyle, FALSE);

	// 登録したウィンドウクラスを作成
	m_hWnd = CreateWindow(
		ClassName,                          // クラス名
		WindowTitle,                        // ウィンドウタイトル
		dwStyle,                            // スタイル (WS_OVERLAPPEDWINDOW)
		CW_USEDEFAULT, CW_USEDEFAULT,       // 初期表示位置 (システム任せ)
		windowRect.right - windowRect.left,	// ウィンドウの幅
		windowRect.bottom - windowRect.top,	// ウィンドウの高さ
		NULL,                               // 親ウィンドウ (今回はなし)
		NULL,                               // メニュー (今回はなし)
		m_hInstance,                        // インスタンスハンドル
		NULL
	);

	if (m_hWnd)
	{
		ShowWindow(m_hWnd, m_nCmdShow);		// ウィンドウ表示
		UpdateWindow(m_hWnd);				// 即座に描画
		return true;						// ウィンドウが作成できれば、正常終了
	}

	// ウィンドウが作成できていないので、falseを返す
	return false;
}