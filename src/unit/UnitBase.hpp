// UnitBase class

#pragma once

# include <queue>
# include "../dx11/DirectX.hpp"
# include "../dx11/Texture.hpp"
# include "../core/Config.hpp"

class Direct3D;
class FieldMap;
class UnitManager;

class UnitBase
{
public:

	// デストラクタ
	~UnitBase() = default;

	// 更新処理
	void update();

	// 描画処理
	void draw() const;

	// 行動終了時の処理
	void onSelectedCommand(const Config::UISettings::Command& selectedCommand);

	// 位置情報の取得
	Config::MapSettings::GridPosition getUnitPosition() const;

	// 現在のユニットの状態
	Config::UnitSettings::UnitState unitState;
	
protected:

	// コンストラクタ
	UnitBase();

	// 初期化処理
	void initialize();

	// 頂点情報の作成処理
	std::vector<Util::Vertex> createVertices() const;

	// 各種パラメータ設定
	virtual void setParameter(const Config::MapSettings::GridPosition& spawnPosition) = 0;

	// 指定座標に瞬間移動する処理
	void setPosition(const Config::MapSettings::GridPosition& targetPosition);

	// 移動処理
	void gridMove();

	// 敵ユニットAI: ターゲットを決定し、移動先/攻撃を判断する
	virtual void decideAction() {};

	// Direct3Dクラスのインスタンス
	Direct3D& m_direct3D;

	// マップ上のアイコンテクスチャ
	Texture m_unitIconTexture;

	// アイコンの色
	DirectX::XMFLOAT4 m_iconColor;

	// 頂点数
	UINT m_vertexCount;

	// 頂点バッファ
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_vertexBuffer;

	// ユニットのタイプ
	Config::UnitSettings::UnitType m_unitType;

	// ユニットのパラメータ
	Config::UnitSettings::UnitParameter m_unitParameter;

	// ユニットの座標(グリッド位置)
	Config::MapSettings::GridPosition m_unitPosition;

	// 移動前のユニットの座標
	Config::MapSettings::GridPosition m_prevPosition;

	// 現在地から他のすべてのマスへの距離を格納する二次元配列
	std::vector<std::vector<int>> m_distanceGrid;

	// 現在地から目的地までの経路を格納する配列
	std::deque<Config::MapSettings::GridPosition> m_movementPath;

	// グリッド移動アニメーションの間隔
	float m_gridMoveTimer;
};