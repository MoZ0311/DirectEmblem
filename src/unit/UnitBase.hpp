// UnitBase class

#pragma once

# include <queue>
# include "../dx11/DirectX.hpp"
# include "../dx11/Texture.hpp"
# include "../core/Config.hpp"

class Direct3D;
class FieldMap;

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
	void onFinishActed(const Config::UISettings::Command& selectedCommand);

	// 位置情報の取得
	Config::MapSettings::GridPosition getUnitPosition() const;

	// ユニット状態の取得
	Config::UnitSettings::UnitState getUnitState() const;
	
protected:

	// コンストラクタ
	UnitBase();

	// 初期化処理
	void initialize();

	// 頂点情報の作成処理
	std::vector<Util::Vertex> createVertices() const;

	// 各種パラメータ設定
	virtual void setParameter() = 0;

	// 指定座標に瞬間移動する処理
	void setPosition(const Config::MapSettings::GridPosition& targetPosition);

	// 移動処理
	void gridMove();

	// Direct3Dクラスのインスタンス
	Direct3D& m_direct3D;

	// FieldMapクラスのインスタンス
	FieldMap& m_fieldMap;

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

	// 現在地から目的地までの経路を格納する配列
	std::deque<Config::MapSettings::GridPosition> m_movementPath;

	// 現在のユニットの状態
	Config::UnitSettings::UnitState m_unitState;

	// グリッド移動アニメーションの間隔
	float m_gridMoveTimer;
};