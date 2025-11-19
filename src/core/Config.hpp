// Config

# pragma once

# include <windows.h>
# include <map>
# include <directxmath.h>

namespace Util
{
	// 十分に巨大な数
	static inline constexpr int INF{ 999 };

	struct Vertex
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT4 color;
		DirectX::XMFLOAT2 uv;
	};

	struct Vec2
	{
		float x;	// x座標(浮動小数点数)
		float y;	// y座標(浮動小数点数)

		// 演算子オーバーロード
		auto operator<=>(const Vec2&) const = default;
	};

	struct ObjectConstants
	{
		// ワールド行列
		DirectX::XMFLOAT4X4 worldMatrix;

		// ビュー行列
		DirectX::XMFLOAT4X4 viewMatrix;

		// プロジェクション行列
		DirectX::XMFLOAT4X4 projectionMatrix;

		// 色データ
		DirectX::XMFLOAT4 color;
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

	namespace UISettings
	{
		// 頂点座標定義
		static inline constexpr float CommandUILeft{ 0.7f };
		static inline constexpr float CommandUIRight{ 0.98f };

		static inline constexpr float UIWidth{ CommandUIRight - CommandUILeft };
		static inline constexpr float CommandUIHeight{ (UIWidth * AspectRatio) };

		static inline constexpr float CommandUIBottom{ -0.967f };
		static inline constexpr float CommandUITop{ CommandUIBottom + CommandUIHeight };

		static inline constexpr float UIHighlightHeight{ CommandUIHeight / 4.0f };

		static inline constexpr float TileUIHeight{ 1.4f };

		enum class Command
		{
			Attack,	// 攻撃コマンド
			Skill,	// 戦技コマンド
			Item,	// 持ち物コマンド
			Wait,	// 待機コマンド

			None	// コマンド未選択
		};
	}

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

		// タイルと侵入コストの対応表
		static inline const std::map<TileType, int> TileAccessCost
		{
			{ TileType::Grass, 1 },
			{ TileType::Forest, 2 },
			{ TileType::Fence, 99 },
			{ TileType::Water, 99 },
		};

		// グリッド座標上の位置
		struct GridPosition
		{
			int x;	// x座標(整数値)
			int y;	// y座標(整数値)

			// 演算子オーバーロード
			auto operator<=>(const GridPosition&) const = default;

			// マンハッタン距離の算出
			inline int manhattanDistanceFrom(const GridPosition& from) const
			{
				return std::abs(x - from.x) + std::abs(y - from.y);
			}
		};

		// グリッド座標における上下左右
		static inline constexpr GridPosition GridOffset[4]{
			{ 0, -1 },	// グリッドの上方向
			{ -1, 0 },	// グリッドの左方向
			{ 1, 0 },	// グリッドの右方向
			{ 0, 1 },	// グリッドの下方向
		};

		// 無効なグリッド座標
		static inline constexpr GridPosition InvalidPosition{ -1, -1 };

		// マップの幅
		static inline constexpr int MapWidth{ 20 };

		// マップの高さ
		static inline constexpr int MapHeight{ 16 };

		// マップの開始x座標
		static inline constexpr float MapStartX{ -TileWidth * MapWidth / 2.0f };

		// マップの開始y座標
		static inline constexpr float MapStartY{ TileHeight * MapHeight / 2.0f };
	}

	namespace UnitSettings
	{
		enum class UnitType
		{
			None,	// 無職

			Sword,	// 剣使い
			Spear,	// 槍使い
			Axe,	// 斧使い
			Bow,	// 弓使い

			Enemy,	// 敵
			EnemyHood
		};

		struct UnitParameter
		{
			int mobility{ 3 };		// 移動力
			int maxHealth;			// 最大HP
			int currentHealth;		// 現在のHP
			int attackPower;		// 攻撃力
			int defence;			// 防御力

			int attackRange{ 1 };	// 射程
		};

		enum class UnitState
		{
			None,				// 選択前、何もしていない
			StandBy,			// 選択後、移動待ち
			Moving,				// 移動中
			Acting,				// 行動中
			Waiting,			// 行動終了

			Attacking,			// 攻撃中

			EnemyTurn			// 敵軍ターン中
		};

		// グリッド移動の間隔
		static inline constexpr float GridMoveInterval{ 0.1f };
	}

	namespace BattleSettings
	{
		enum class GamePhase
		{
			PlayerTurn,
			EnemyTurn,
		};
	}
}

namespace SceneSettings
{
	// 各シーンの列挙
	enum class Scene
	{
		Title,	// タイトルシーン
		Game,	// ゲームシーン
		Clear,	// クリアシーン
		Over	// ゲームオーバーシーン
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

	// クリア画面の画像のファイルパス
	static inline constexpr WCHAR ClearImagePath[]{ L"assets/images/background_clear.png" };

	// ゲームオーバー画面の画像のファイルパス
	static inline constexpr WCHAR OverImagePath[]{ L"assets/images/background_over.png" };

	// タイトルロゴ画像のファイルパス
	static inline constexpr WCHAR TitleLogoPath[]{ L"assets/images/logo.png" };

	// ハイライト用テクスチャのファイルパス
	static inline constexpr WCHAR HighlightTexturePath[]{ L"assets/images/highlight.png" };

	// タイルマップ画像のファイルパス
	static inline constexpr WCHAR TileSheetPath[]{ L"assets/images/tile_sheet_pack.png" };

	// スライムのアイコン画像のファイルパス
	static inline constexpr WCHAR SlimeIconPath[]{ L"assets/images/icon_slime.png" };

	// プレイヤー歩兵のアイコン画像のファイルパス
	static inline constexpr WCHAR PlayerIconPath[]{ L"assets/images/icon_player.png" };

	// プレイヤー弓兵のアイコン画像のファイルパス
	static inline constexpr WCHAR PlayerArcherIconPath[]{ L"assets/images/icon_playerhood.png" };

	// 敵歩兵のアイコン画像のファイルパス
	static inline constexpr WCHAR EnemyIconPath[]{ L"assets/images/icon_enemy.png" };

	// 敵弓兵のアイコン画像のファイルパス
	static inline constexpr WCHAR EnemyArcherIconPath[]{ L"assets/images/icon_enemyhood.png" };

	// コマンドUI画像のファイルパス
	static inline constexpr WCHAR CommandUIPath[]{ L"assets/images/command.png" };

	// 草原のタイルUI画像のファイルパス
	static inline constexpr WCHAR GrassUIPath[]{ L"assets/images/ui_grass.png" };

	// 森のタイルUI画像のファイルパス
	static inline constexpr WCHAR ForestUIPath[]{ L"assets/images/ui_forest.png" };

	// 柵のタイルUI画像のファイルパス
	static inline constexpr WCHAR FenceUIPath[]{ L"assets/images/ui_fence.png" };

	// 水のタイルUI画像のファイルパス
	static inline constexpr WCHAR WaterUIPath[]{ L"assets/images/ui_water.png" };

	// プレイヤー歩兵のユニットUI画像のファイルパス
	static inline constexpr WCHAR PlayerUIPath[]{ L"assets/images/ui_player.png" };

	// プレイヤー弓兵のユニットUI画像のファイルパス
	static inline constexpr WCHAR PlayerHoodUIPath[]{ L"assets/images/ui_playerhood.png" };

	// 敵歩兵のユニットUI画像のファイルパス
	static inline constexpr WCHAR EnemyUIPath[]{ L"assets/images/ui_enemy.png" };

	// 敵弓兵のユニットUI画像のファイルパス
	static inline constexpr WCHAR EnemyHoodUIPath[]{ L"assets/images/ui_enemyhood.png" };
}