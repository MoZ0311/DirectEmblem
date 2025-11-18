// BattleManager class

#pragma once

class BattleManager
{
public:

	// シングルトンインスタンスの生成/取得
	static BattleManager& GetInstance();

	// 更新処理
	void update();

private:

	// コンストラクタ
	BattleManager();

	// コピーコンストラクタを削除
	BattleManager(const BattleManager&) = delete;
};