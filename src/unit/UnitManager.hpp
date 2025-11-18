// UnitManager class

#pragma once

# include <memory>
# include <vector>
# include "../core/Config.hpp"

class UnitBase;

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

	// 自軍ユニット配列の取得
	const std::vector<std::unique_ptr<UnitBase>>& getPlayerUnitArray() const;

	// 敵軍ユニット配列の取得
	const std::vector<std::unique_ptr<UnitBase>>& getEnemyUnitArray() const;

	// 全ユニットの状態リセット
	void resetAllUnitState();

private:

	// コンストラクタ
	UnitManager();

	// コピーコンストラクタを削除
	UnitManager(const UnitManager&) = delete;

	// 初期化処理
	void initialize();

	// 自軍ユニット配列
	std::vector<std::unique_ptr<UnitBase>> m_playerUnitArray;

	// 敵軍ユニット配列
	std::vector<std::unique_ptr<UnitBase>> m_enemyUnitArray;
};