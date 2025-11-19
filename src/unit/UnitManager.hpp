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

	// リセット処理
	void resetState();

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

	// 指定座標のユニット情報の取得
	Config::UnitSettings::UnitType getUnitTypeAtPosition(const Config::MapSettings::GridPosition& position) const;

	// ユニットの追加処理
	void generateUnits();

	// 全ユニットの状態リセット
	void resetAllUnitState();

	// 任意の座標上のユニットを消し去る
	void removeUnitAtPosition(const Config::MapSettings::GridPosition& position);

private:

	// コンストラクタ
	UnitManager();

	// コピーコンストラクタを削除
	UnitManager(const UnitManager&) = delete;

	// 自軍ユニット配列
	std::vector<std::unique_ptr<UnitBase>> m_playerUnitArray;

	// 敵軍ユニット配列
	std::vector<std::unique_ptr<UnitBase>> m_enemyUnitArray;
};