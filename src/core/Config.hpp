// Config

# pragma once

# include <windows.h>
# include <memory>
# include <vector>
# include <map>
# include <string>
# include <directxmath.h>
# include <algorithm>

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
		// タイルの幅
		static inline constexpr float TileWidth{ 0.068f };

		// タイルの高さ
		static inline constexpr float TileHeight{ TileWidth * AspectRatio };

		// タイルの種類
		enum class TileType
		{
			Grass,	// 草原
			Forest,	// 森
			Fence,	// 柵
			Water,	// 水
		};

		// タイルとテクスチャアトラスのuv対応表
		static inline const std::map<TileType, DirectX::XMFLOAT2> TileUVMap
		{
			{ TileType::Grass,  { 0, 0 } }, // 0列目, 0行目
			{ TileType::Forest, { 1, 0 } }, // 1列目, 0行目
			{ TileType::Fence,  { 0, 1 } }, // 0列目, 1行目
			{ TileType::Water,  { 1, 1 } }  // 1列目, 1行目
		};

		// タイルと侵入可否の対応表
		static inline const std::map<TileType, bool> TileAccessible
		{
			{ TileType::Grass, true },	// 草原, 侵入可
			{ TileType::Forest, true },	// 　森, 侵入可
			{ TileType::Fence, false },	// 　柵, 侵入不可
			{ TileType::Water, false }	// 　水, 侵入不可
		};

		// グリッド座標上の位置
		struct GridPosition
		{
			int x;
			int y;
		};

		// マップの幅
		static inline constexpr int MapWidth{ 20 };

		// マップの高さ
		static inline constexpr int MapHeight{ 16 };

		// マップの開始x座標
		static inline constexpr float MapStartX{ -TileWidth * MapWidth / 2 };

		// マップの開始y座標
		static inline constexpr float MapStartY{ TileHeight * MapHeight / 2 };
	}

	namespace UnitSettings
	{
		enum class UnitType
		{
			None,	// 無職

			Sword,	// 剣使い
			Spear,	// 槍使い
			Axe,	// 斧使い
			Bow		// 弓使い
		};

		struct UnitStatus
		{
			int mobility;		// 移動力
			int maxHealth;		// 最大HP
			int currentHealth;	// 現在のHP
			int attackPower;	// 攻撃力
			int defence;		// 防御力
		};
	}
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

namespace FilePath
{
	// シェーダーのファイルパス
	static inline constexpr WCHAR ShaderFileName[]{ L"shader/Shader.hlsl" };

	// タイトル画像のファイルパス
	static inline constexpr WCHAR TitleImagePath[]{ L"assets/images/background_title.png" };

	// タイトルロゴ画像のファイルパス
	static inline constexpr WCHAR TitleLogoPath[]{ L"assets/images/logo.png" };

	// ハイライト用テクスチャのファイルパス
	static inline constexpr WCHAR HighlightTexturePath[]{ L"assets/images/highlight.png" };

	// タイルマップ画像のファイルパス
	static inline constexpr WCHAR TileSheetPath[]{ L"assets/images/tile_sheet_pack.png" };

	// スライムのアイコン画像のファイルパス
	static inline constexpr WCHAR SlimeIconPath[]{ L"assets/images/icon_slime.png" };

	// キャラのスプライトシートのファイルパス
	static inline constexpr WCHAR CharacterSheetPath[]{ L"assets/images/character_sheet.png" };
}