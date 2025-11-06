// UIManager class

#pragma once

class UIManager
{
public:

	// シングルトンインスタンスの生成/取得
	static UIManager& GetInstance();

	// 更新処理
	void update();

	// 描画処理
	void draw() const;

private:

	// コンストラクタ
	UIManager();
};