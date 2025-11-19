// UnitEnemyBow class

# include "UnitEnemyBow.hpp"

# include "../battle/BattleManager.hpp"
# include "../util/PathFinder.hpp"

using namespace Config::UnitSettings;
using namespace Config::MapSettings;
using namespace FilePath;

UnitEnemyBow::UnitEnemyBow(const Config::MapSettings::GridPosition& spawnPosition)
{
    setParameter(spawnPosition);
}

void UnitEnemyBow::setParameter(const Config::MapSettings::GridPosition& spawnPosition)
{
    setPosition(spawnPosition);
    m_unitType = UnitType::EnemyHood;
    m_unitIconTexture = Texture{ EnemyArcherIconPath };

    m_unitParameter.mobility = 3;
    m_unitParameter.attackRange = 2;
}

void UnitEnemyBow::decideAction()
{
    // ターゲットの決定
    const GridPosition targetPosition{ BattleManager::GetInstance().findNearestPlayerUnit(m_unitPosition) };

    // ターゲットが見つからない場合は待機して終了
    if (targetPosition == InvalidPosition)
    {
        unitState = UnitState::Waiting;
        m_iconColor = { 0.6f, 0.6f, 0.6f, 1.0f };
        return;
    }

    // 移動可能範囲のコストマップを作成
    m_distanceGrid = PathFinder::CalculateDistanceGrid(m_unitPosition, m_unitParameter.mobility);

    int bestDistanceToTarget{ INT_MAX };
    GridPosition bestMovePosition{ m_unitPosition };

    // 射程限界のマスが見つかったか
    bool foundPerfectRange = false;

    // 全移動可能マスを走査
    for (int y = 0; y < MapHeight; ++y)
    {
        for (int x = 0; x < MapWidth; ++x)
        {
            const int distanceCost = m_distanceGrid[y][x];

            // 移動可能なマスか？
            if (distanceCost > 0 && distanceCost <= m_unitParameter.mobility)
            {
                const GridPosition currentMovePos{ x, y };

                // 移動候補地からターゲットまでのマンハッタン距離
                int distanceToTarget{ targetPosition.manhattanDistanceFrom(currentMovePos) };

                // 射程範囲内の時
                if (distanceToTarget <= m_unitParameter.attackRange)
                {
                    // 射程限界の時
                    if (distanceToTarget == m_unitParameter.attackRange)
                    {
                        // 射程限界のマスを最優先のターゲットとして設定
                        // 初めて見つけた場合、または、既存の射程限界マスより移動コストが低い場合などを考慮
                        if (!foundPerfectRange || (foundPerfectRange && distanceCost < m_distanceGrid[bestMovePosition.y][bestMovePosition.x]))
                        {
                            foundPerfectRange = true;
                            bestDistanceToTarget = m_unitParameter.attackRange; // スコアを固定
                            bestMovePosition = currentMovePos;
                        }
                    }

                    // 射程限界でない時
                    if (!foundPerfectRange)
                    {
                        // 射程限界のマスが見つかっていない間は、攻撃可能マスの中で「最も遠いマス」を優先
                        if (distanceToTarget > bestDistanceToTarget || bestDistanceToTarget == INT_MAX)
                        {
                            bestDistanceToTarget = distanceToTarget;
                            bestMovePosition = currentMovePos;
                        }
                    }
                }
                else if (!foundPerfectRange)
                {
                    // ターゲットに最も近づけるマスを探す
                    if (distanceToTarget < bestDistanceToTarget)
                    {
                        bestDistanceToTarget = distanceToTarget;
                        bestMovePosition = currentMovePos;
                    }
                }
            }
        }
    }
    // 移動経路を作成
    m_movementPath = PathFinder::CreateMovementPath(m_distanceGrid, bestMovePosition);
    unitState = UnitState::Moving; // 移動処理へ遷移
}