# include "src/core/Application.hpp"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	// Application classを実体化
	Application app{ hInstance, nCmdShow };

	// ウィンドウ作成処理
	if (!app.initialize())
	{
		// 失敗時、return
		return -1;
	}

	// ゲームループ
	const MSG msg{ app.run() };

	// wParamを戻り値として終了
	return static_cast<int>(msg.wParam);
}