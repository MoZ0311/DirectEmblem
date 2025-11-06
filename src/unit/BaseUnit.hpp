// BaseUnit class

#pragma once

# include "../dx11/DirectX.hpp"
# include "../map/FieldMap.hpp"

class BaseUnit
{
public:

	// デストラクタ
	~BaseUnit() = default;

	// 更新処理
	void update();

	// 描画処理
	void draw() const;

	// 位置情報の取得
	Config::MapSettings::GridPosition getUnitPosition() const;
	
protected:

	// コンストラクタ
	BaseUnit();

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

	// 幅優先探索による移動範囲の計算
	void calculateMovementRange();

	// 目的地に向けた経路の作成処理
	void createMovementPath(const Config::MapSettings::GridPosition& targetPosition);

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

	// 始点からの距離の二次元配列
	std::vector<std::vector<int>> m_distanceGrid;

	// 現在地から目的地までの経路を格納する配列
	std::deque<Config::MapSettings::GridPosition> m_movementPath;

	// 選択中であるか
	bool m_hasSelected;

	// 移動済みであるか
	bool m_hasMoved;

	// 行動済みであるか
	bool m_hasActed;

	// グリッド移動アニメーションの間隔
	float m_gridMoveTimer;
};