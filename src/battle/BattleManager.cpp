// BattleManager class

# include "BattleManager.hpp"

# include "../unit/UnitBase.hpp"
# include "../unit/UnitManager.hpp"
# include "../ui/UIManager.hpp"

using namespace Config::UnitSettings;
using namespace Config::BattleSettings;
using namespace Config::UISettings;

BattleManager::BattleManager()
	: m_currentPhase{ GamePhase::PlayerTurn }
	, m_selectedCommand{ Command::None }
	, currentUnitState{ UnitState::None }
	, m_currentEnemyIndex{ 0 }
{

}

BattleManager& BattleManager::GetInstance()
{
	static BattleManager instance;
	return instance;
}

void BattleManager::update()
{
	switch (m_currentPhase)
	{
	case GamePhase::PlayerTurn:
		handlePlayerUnit();
		break;

	case GamePhase::EnemyTurn:
		handleEnemyUnit();
		break;
	default:
		break;
	}
}

void BattleManager::handlePlayerUnit()
{
	// プレイヤーユニットが全て行動済みかチェック
	bool allPlayerUnitsActed{ true };

	// 自軍ユニットの配列を取得
	const auto& playerUnitArray{ UnitManager::GetInstance().getPlayerUnitArray() };

	// 範囲for文でプレイヤーのユニットを走査
	for (const auto& playerUnit : playerUnitArray)
	{
		// 行動を終えていないユニットを見つけたら、フラグを折る
		if (playerUnit->unitState != UnitState::Waiting)
		{
			allPlayerUnitsActed = false;
		}

		// 行動中のユニットを見つけたら、コマンドに応じた処理を行なう
		if (playerUnit->unitState == UnitState::Acting)
		{
			if (m_selectedCommand == Command::None)
			{
				// コマンド未選択: UI描画フラグを立てる
				UIManager::GetInstance().isDrawingCommandUI = true;
			}
			else
			{
				// コマンド選択後: ユニット側の処理を呼び出し
				playerUnit->onSelectedCommand(m_selectedCommand);
			}
			break;
		}
		UIManager::GetInstance().isDrawingCommandUI = false;
	}

	if (allPlayerUnitsActed)
	{
	   // フェイズを敵ターンに切り替え
	   m_currentPhase = GamePhase::EnemyTurn;

	   // 敵ターン開始時のインデックスをリセット
	   m_currentEnemyIndex = 0;
	}
}

void BattleManager::handleEnemyUnit()
{
	// 敵軍ユニットの配列を取得
	const auto& enemyUnitArray{ UnitManager::GetInstance().getEnemyUnitArray() };

	if (m_currentEnemyIndex < enemyUnitArray.size())
	{
		const auto& currentEnemy = enemyUnitArray[m_currentEnemyIndex];

		// まだ行動していないユニットであれば、AI行動開始のステートに設定する
		if (currentEnemy->unitState == UnitState::None)
		{
			// AI行動決定ステートに遷移させる
			currentEnemy->unitState = UnitState::EnemyTurn;
		}

		// 敵ユニットの行動（移動、攻撃など）が完了したかチェック
		if (currentEnemy->unitState == UnitState::Waiting)
		{
			// 行動が完了したら、次の敵へ
			m_currentEnemyIndex++;
		}
	}
	else
	{
		// 全ての敵ユニットの行動が終了

		// TODO: 全ユニットのステートをリセット (Waiting -> None)
		UnitManager::GetInstance().resetAllUnitState();

		// フェイズをプレイヤーターンに切り替え
		m_currentPhase = GamePhase::PlayerTurn;
	}
}

GamePhase BattleManager::getCurrentPhase() const
{
	return m_currentPhase;
}

void BattleManager::setSelectedCommand(const Command& selectedCommand)
{
	m_selectedCommand = selectedCommand;
}