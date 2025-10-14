// Config

# pragma once

# include <windows.h>
# include <memory>
// 可変長配列
# include <vector>
# include <directxmath.h>

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
		static inline constexpr UINT MapWidth{ 11 };

		// マップの高さ
		static inline constexpr UINT MapHeight{ 11 };

		// タイルの種類
		enum class TileType
		{
			Grass,
			Forest,
			Fence,
			Water,

			TileMax	// 要素数取得のためのダミー
		};
	}

	static inline constexpr WCHAR ShaderFileName[]{ L"shader/Shader.hlsl" };
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