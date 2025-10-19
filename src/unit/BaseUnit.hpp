// BaseUnit class

#pragma once

# include "../dx11/DirectX.hpp"

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

	// グリッド座標の変更処理
	void setUnitPosition(const Config::MapSettings::GridPosition& targetPoint);

	// Direct3Dクラスのインスタンス
	Direct3D& m_direct3D;

	// マップ上のアイコンテクスチャ
	Texture m_unitIconTexture;

	// 頂点数
	UINT m_vertexCount;

	// 頂点バッファ
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_vertexBuffer;

	// ユニットのタイプ
	Config::UnitSettings::UnitType m_unitType;

	// ユニットのステータス
	Config::UnitSettings::UnitStatus m_unitStatus;

	// ユニットの座標(グリッド位置)
	Config::MapSettings::GridPosition m_unitPosition;

	// 移動済みであるか
	bool m_hasMoved;

	// 行動済みであるか
	bool m_hasActed;
};