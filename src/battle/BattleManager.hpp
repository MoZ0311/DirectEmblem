// BattleManager class

#pragma once

# include "../core/Config.hpp"

class BattleManager
{
public:

	// シングルトンインスタンスの生成/取得
	static BattleManager& GetInstance();

	// 更新処理
	void update();

	// 現在のフェーズ(ターン)の取得処理
	Config::BattleSettings::GamePhase getCurrentPhase() const;

	// コマンド選択時のユニットに対する操作
	void setSelectedCommand(const Config::UISettings::Command& selectedCommand);

	// 任意に選択したユニットの状態
	Config::UnitSettings::UnitState currentUnitState;

private:

	// コンストラクタ
	BattleManager();

	// コピーコンストラクタを削除
	BattleManager(const BattleManager&) = delete;

	// プレイヤーユニットの処理
	void handlePlayerUnit();

	// 敵ユニットの処理
	void handleEnemyUnit();

	// 現在のフェーズ(ターン)
	Config::BattleSettings::GamePhase m_currentPhase;

	// 選択されたコマンド
	Config::UISettings::Command m_selectedCommand;

	// 敵軍ユニットのインデックス
	int m_currentEnemyIndex;
};