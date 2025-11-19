// BattleManager class

# include "BattleManager.hpp"

# include <random>
# include "../unit/UnitBase.hpp"
# include "../unit/UnitManager.hpp"
# include "../ui/UIManager.hpp"
# include "../map/FieldMap.hpp"

using namespace Config::UnitSettings;
using namespace Config::MapSettings;
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

void BattleManager::resetState()
{
	m_currentPhase = GamePhase::PlayerTurn;
	m_selectedCommand = Command::None;
	currentUnitState = UnitState::None;
	m_currentEnemyIndex = 0;
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

	switch (currentUnitState)
	{
	case UnitState::None:		// 選択前
	case UnitState::Waiting:	// 行動後

		// UI非表示
		UIManager::GetInstance().isDrawingCommandUI = false;
		break;

	case UnitState::Acting:
		// UI表示
		UIManager::GetInstance().isDrawingCommandUI = true;
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

		// 行動中のユニットを見つけたら、コマンドに応じた処理を実行
		if (playerUnit->unitState == UnitState::Acting)
		{
			// None以外: 何らかのコマンドが選択された
			if (m_selectedCommand != Command::None)
			{
				// デバッグ処理として戦技と持ち物を封印
				if (m_selectedCommand != Command::Attack &&
					m_selectedCommand != Command::Wait)
				{
					break;
				}

				// コマンド選択後: ユニット側の処理を呼び出す
				playerUnit->onSelectedCommand(m_selectedCommand);
			}			
		}
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
		m_currentEnemyIndex = 0;
	}
}

GridPosition BattleManager::findNearestPlayerUnit(const GridPosition& startPosition) const
{
	// 最も近い座標を無効座標で宣言しておく
	GridPosition closestPosition{ InvalidPosition };

	// その時の距離も、intの最大値にしておく
	int minDistance{ INT_MAX };

	// 自軍ユニットの配列を取得
	const auto& playerUnitArray{ UnitManager::GetInstance().getPlayerUnitArray() };

	// プレイヤーユニット配列を走査
	for (const auto& playerUnit : playerUnitArray)
	{
		const GridPosition playerPos{ playerUnit->getUnitPosition() };

		// ユニット間の距離を計算
		int distance{ playerPos.manhattanDistanceFrom(startPosition) };

		if (distance < minDistance)
		{
			minDistance = distance;
			closestPosition = playerPos;
		}
	}

	return closestPosition;
}

void BattleManager::executeAttack(const Config::MapSettings::GridPosition& targetPosition)
{
	// ユニットの存在情報を事前に取得
	const auto& unitStandingGrid{ UnitManager::GetInstance().getUnitStandingGrid() };

	// 目標地点にユニットが存在する時
	if (unitStandingGrid[targetPosition.y][targetPosition.x])
	{
		// そのユニットのいるタイルを取得
		const auto& mapData{ FieldMap::GetInstance().getMapData() };
		const TileType targetTile{ mapData[targetPosition.y][targetPosition.x] };
		
		// 命中率の基礎値は100%
		int probavility{ 100 };

		// 森にいる時、命中率は下がる
		if (targetTile == TileType::Forest)
		{
			probavility = 50;
		}

		// 乱数エンジン作成
		static std::mt19937 generator(std::random_device{}());
		std::uniform_int_distribution<> distribution(0, 99);

		// 乱数を作成
		int randomNumber{ distribution(generator) };
		if (randomNumber < probavility)
		{
			UnitManager::GetInstance().removeUnitAtPosition(targetPosition);
		}
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