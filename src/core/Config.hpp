// Config

# pragma once

# include <windows.h>

namespace Config
{
	// ウィンドウクラス名
	static inline constexpr WCHAR ClassName[]{ L"MainWindow" };

	// ウィンドウのタイトル
	static inline constexpr WCHAR WindowTitle[]{ L"ダイレクトエムブレム" };

	// ウィンドウの幅
	static inline constexpr UINT WindowWidth{ 1280 };

	// ウィンドウの高さ
	static inline constexpr UINT WindowHeight{ 720 };

	// リフレッシュレート
	static inline constexpr UINT RefreshRate{ 60 };

	namespace MapSettings
	{
		// マップの幅
		static inline constexpr UINT MapWidth{ 16 };

		// マップの高さ
		static inline constexpr UINT MapHeight{ 16 };

		// タイルの種類
		enum class TileType
		{
			Grass,
			Forest,
			Water
		};
	}
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

struct ColorF
{
	float rgba[4];
};

struct Position
{
	float xyz[3];
};

struct Vertex
{
	Position position;
	ColorF color;
};

struct Vec2
{
	float x;
	float y;
};