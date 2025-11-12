// UnitManager class

#pragma once

# include <memory>
# include <vector>
# include "../core/Config.hpp"

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

	// コマンド選択時のユニットに対する操作
	void onCommandSelected(const Config::UISettings::Command& selectedCommand);

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

	// 選択されたコマンド
	Config::UISettings::Command m_selectedCommand;
};