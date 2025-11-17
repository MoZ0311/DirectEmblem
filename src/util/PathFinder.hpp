// PathFinder

# pragma once

# include <vector>
# include <queue>
# include "../core/Config.hpp"
# include "../map/FieldMap.hpp"
# include "../unit/UnitManager.hpp"

struct PathFinder
{
	// 始点から全てのマスに対しての距離を算出する処理
	static inline std::vector<std::vector<int>> CalculateDistanceGrid(const Config::MapSettings::GridPosition& startPos, const int mobility)
	{
		// 距離配列を巨大な値で初期化
		std::vector<std::vector<int>> distanceGrid{ Config::MapSettings::MapHeight, std::vector<int>(Config::MapSettings::MapWidth, Util::INF) };

		// 移動コストと座標のペアを定義
		using Pair = std::pair<int, Config::MapSettings::GridPosition>;

		// 探索用のキュー作成
		std::priority_queue<Pair, std::vector<Pair>, std::greater<Pair>> searchQueue{};

		// 現在地の距離を0にする
		distanceGrid[startPos.y][startPos.x] = 0;

		// 探索キューに初期位置を渡す
		searchQueue.emplace(0, startPos);

		// 事前にマップデータとユニット位置を取得
		const std::vector<std::vector<int>>& mapData{ FieldMap::GetInstance().getMapData() };
		const std::vector<std::vector<bool>>& unitStandingGrid{ UnitManager::GetInstance().getUnitStandingGrid() };

		// キューが空になる(これ以上探索できない)まで繰り返す
		while (!searchQueue.empty())
		{
			// キューの先頭を取り出す
			const Pair searchPair{ searchQueue.top() };
			searchQueue.pop();

			const int searchDistance{ searchPair.first };
			const Config::MapSettings::GridPosition searchPosition{ searchPair.second };

			// 最短でなければ、処理しない
			if (searchDistance > distanceGrid[searchPosition.y][searchPosition.x])
			{
				continue;
			}

			// offsetで定義した四方向に範囲for文でアクセス
			for (const auto& offset : Config::MapSettings::GridOffset)
			{
				// アクセス先のグリッド座標を定義
				const Config::MapSettings::GridPosition nextPosition{
					searchPosition.x + offset.x,
					searchPosition.y + offset.y
				};

				// マップの境界チェック
				if (nextPosition.x < 0 || nextPosition.x >= Config::MapSettings::MapWidth ||
					nextPosition.y < 0 || nextPosition.y >= Config::MapSettings::MapHeight)
				{
					// 範囲外アクセス時に、continue
					continue;
				}

				// ユニットがいるマスは、侵入不可としてスキップ
				if (unitStandingGrid[nextPosition.y][nextPosition.x])
				{
					continue;
				}

				// mapDataのint型をTileTypeに変換
				const Config::MapSettings::TileType nextTileType{ static_cast<Config::MapSettings::TileType>(mapData[nextPosition.y][nextPosition.x]) };

				// 侵入コストを対応表から取得
				const int accessCost{ Config::MapSettings::TileAccessCost.at(nextTileType) };

				// 新しい距離を計算
				const int newDistance{ searchDistance + accessCost };

				if (newDistance <= mobility && newDistance < distanceGrid[nextPosition.y][nextPosition.x])
				{
					// m_distanceGridを更新
					distanceGrid[nextPosition.y][nextPosition.x] = newDistance;

					// 次の探索先をキューに追加
					searchQueue.emplace(newDistance, nextPosition);
				}
			}
		}

		// 完成した二次元配列を返す
		return distanceGrid;
	}

	// 現在地から目的地までのパスを作成する処理
	static inline std::deque<Config::MapSettings::GridPosition> CreateMovementPath(
		const std::vector <std::vector<int>>& distanceGrid,
		const Config::MapSettings::GridPosition& targetPos)
	{
		// 経路を格納するコンテナ
		std::deque<Config::MapSettings::GridPosition> path{};

		// 目的地から探索を始める
		Config::MapSettings::GridPosition currentPosition{ targetPos };
		int currentDistance{};

		// 事前にマップデータを取得 (コスト計算に必要)
		// FieldMap::GetInstance().getMapData() の戻り値が正しい前提
		const std::vector<std::vector<int>>& mapData{ FieldMap::GetInstance().getMapData() };

		// 目的地から始点（距離 0）まで探索
		while (true)
		{
			// 現在地の距離を取得
			currentDistance = distanceGrid[currentPosition.y][currentPosition.x];

			// 現在地をパスに格納
			path.push_back(currentPosition);

			// 始点（距離 0）に到達したら終了
			if (currentDistance == 0)
			{
				break;
			}

			// 次のノードを見つけるためのフラグと変数
			bool foundNext = false;
			Config::MapSettings::GridPosition nextStepPosition = currentPosition;

			// 範囲for文で、隣接4マスの始点からの距離を評価
			for (const auto& offset : Config::MapSettings::GridOffset)
			{
				// 次の探索目標を設定
				const Config::MapSettings::GridPosition nextPosition{
					currentPosition.x + offset.x,
					currentPosition.y + offset.y
				};

				// マップの境界チェック
				if (nextPosition.x < 0 || nextPosition.x >= Config::MapSettings::MapWidth ||
					nextPosition.y < 0 || nextPosition.y >= Config::MapSettings::MapHeight)
				{
					continue;
				}

				// 次のマスへの距離を算出
				const int nextDistance{ distanceGrid[nextPosition.y][nextPosition.x] };

				// 次のマスが到達不能（距離無限大など）の場合はスキップ
				// ただし、ダイクストラの結果として nextDistance >= currentDistance はありえないはず
				// nextDistance >= currentDistance は、現在のノードが最短経路上のノードではないことを示唆します。
				if (currentDistance < nextDistance)
				{
					continue;
				}

				// nextDistance が -1 など無効な値の場合のチェックも考慮すべきですが、ここでは省略します。

				// mapDataのint型をTileTypeに変換
				const Config::MapSettings::TileType nextTileType{ static_cast<Config::MapSettings::TileType>(mapData[nextPosition.y][nextPosition.x]) };

				// 侵入コストを対応表から取得
				const int accessCost{ Config::MapSettings::TileAccessCost.at(nextTileType) };

				// 現在の距離が、隣接ノードの距離 + 移動コストと一致するかチェック
				if (nextDistance + accessCost == currentDistance)
				{
					// 条件を満たす隣接ノードを見つけた
					nextStepPosition = nextPosition;
					foundNext = true;
					// 最初に条件を満たしたノードを次のステップとする (これで OK)
					break;
				}
			}

			// 次のステップが見つからなかった場合の処理 (ロジック破綻)
			if (!foundNext && currentDistance != 0) {
				// 始点以外で次のステップが見つからないのは、distanceGridの計算が不正か、
				// 到達不能なグリッドにターゲットが設定されたことを意味します。
				// エラー処理を考慮すべきですが、ここではそのまま break します。
				break;
			}

			// 次のステップを現在の位置にする
			currentPosition = nextStepPosition;
		}

		// パスは目的地から始点になっているため、反転して始点から目的地にする
		std::reverse(path.begin(), path.end());

		// 完成した経路を返す
		return path;
	}
};