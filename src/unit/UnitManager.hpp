// UnitManager class

#pragma once

# include <memory>
# include <vector>

class UnitBase;
class UIManager;

class UnitManager
{
public:

	// シングルトンインスタンスの生成/取得
	static UnitManager& GetInstance();

	// 更新処理
	void update();

	// 描画処理
	void draw() const;

	// ユニットの存在二次元配列の取得
	std::vector<std::vector<bool>> getUnitStandingGrid() const;

	// 任意のユニットが移動中であるか
	bool isUnitMoving;

private:

	// コンストラクタ
	UnitManager();

	// コピーコンストラクタを削除
	UnitManager(const UnitManager&) = delete;

	// 自軍ユニット配列
	std::vector<std::unique_ptr<UnitBase>> m_playerUnitArray;

	// 敵軍ユニット配列
	std::vector<std::unique_ptr<UnitBase>> m_enemyUnitArray;

	// UI管理クラスのインスタンス
	UIManager& m_uiManager;
};