// Config

# pragma once

# include <windows.h>
# include <memory>
# include <vector>
# include <map>
# include <string>
# include <directxmath.h>

namespace Util
{
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

	struct Point
	{
		UINT x;
		UINT y;
	};
}

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
		static inline constexpr int MapHeight{ 16 };

		// タイルの幅
		static inline constexpr float TileWidth{ 0.068f };

		// タイルの高さ
		static inline constexpr float TileHeight{ TileWidth * AspectRatio };

		// タイルの種類
		enum class TileType
		{
			Grass,
			Forest,
			Fence,
			Water,

			// TileMax	// 要素数取得のためのダミー
		};

		// タイルとテクスチャアトラスのuv対応表
		static inline const std::map<TileType, Util::Point> TileUVMap
		{
			{ TileType::Grass,  { 0, 0 } }, // 0列目, 0行目
			{ TileType::Forest, { 1, 0 } }, // 1列目, 0行目
			{ TileType::Fence,  { 0, 1 } }, // 0列目, 1行目
			{ TileType::Water,  { 1, 1 } }  // 1列目, 1行目
		};
	}

	// シェーダーのファイルパス
	static inline constexpr WCHAR ShaderFileName[]{ L"shader/Shader.hlsl" };

	// タイトル画像のファイルパス
	static inline constexpr WCHAR TitleImagePath[]{ L"assets/images/background_title.png" };

	// タイトルロゴ画像のファイルパス
	static inline constexpr WCHAR TitleLogoPath[]{ L"assets/images/logo.png" };

	// タイルマップ画像のファイルパス
	static inline constexpr WCHAR TileSheetPath[]{ L"assets/images/tile_sheet_pack.png" };

	// スライムのアイコン画像のファイルパス
	static inline constexpr WCHAR SlimeIconPath[]{ L"assets/images/icon_slime.png" };

	// キャラのスプライトシートのファイルパス
	static inline constexpr WCHAR CharacterSheetPath[]{ L"assets/images/character_sheet.png" };
}

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