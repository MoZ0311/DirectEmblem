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
	
protected:

	// コンストラクタ
	BaseUnit();

	// 初期化処理
	void initialize();

	// 頂点情報の作成処理
	std::vector<Util::Vertex> createVertices() const;

	// 各種パラメータ設定
	virtual void setParameter() = 0;

	// 幅優先探索の計算
	void calculateDistance();

	// グリッド座標の変更処理
	void setUnitPosition(const Config::MapSettings::GridPosition& targetPoint);

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

	// 現在値からの距離(侵入コスト込)の二次元配列
	std::vector<std::vector<int>> m_distanceGrid;

	// 選択中であるか
	bool m_hasSelected;

	// 移動済みであるか
	bool m_hasMoved;

	// 行動済みであるか
	bool m_hasActed;
};