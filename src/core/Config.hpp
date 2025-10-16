// Config

# pragma once

# include <windows.h>
# include <memory>
# include <vector>
# include <string>
# include <directxmath.h>

namespace Config
{
	// ウィンドウクラス名
	static inline constexpr WCHAR ClassName[]{ L"MainWindow" };

	// ウィンドウのタイトル
	static inline constexpr WCHAR WindowTitle[]{ L"ダイレクトエムブレム" };

	// ウィンドウの幅
	static inline constexpr int WindowWidth{ 1280 };

	// ウィンドウの高さ
	static inline constexpr int WindowHeight{ 720 };

	// 画面の縦横比
	static inline constexpr float AspectRatio{ static_cast<float>(WindowWidth) / WindowHeight };

	// リフレッシュレート
	static inline constexpr UINT RefreshRate{ 60 };

	namespace MapSettings
	{
		// マップの幅
		static inline constexpr int MapWidth{ 20 };

		// マップの高さ
		static inline constexpr int MapHeight{ 20 };

		// タイルの種類
		enum class TileType : int
		{
			Grass,
			Forest,
			Fence,
			Water,

			TileMax	// 要素数取得のためのダミー
		};
	}

	static inline constexpr WCHAR ShaderFileName[]{ L"shader/Shader.hlsl" };
	static inline constexpr WCHAR BackgroundPath[]{ L"assets/images/background_title.png" };
	static inline constexpr WCHAR TileSheetPath[]{ L"assets/images/tiles/tile_sheet.png" };
};

namespace SceneSettings
{
	// 各シーンの列挙
	enum class Scene
	{
		Title,
		Game
	};

	// シーンの切替状態
	enum class TransitionState
	{
		None,		// 何もしていない状態(通常)
		FadingOut,	// フェードアウト中
		FadingIn,	// フェードイン中
		Switching	// シーンの切替中(真っ黒状態)
	};

	struct GameData
	{

	};
}

struct Vertex
{
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT4 color;
	DirectX::XMFLOAT2 uv;
};

struct Vec2
{
	float x;
	float y;
};