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
		// 距離の二次元配列を巨大な値で初期化
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
			if (distanceGrid[searchPosition.y][searchPosition.x] < searchDistance)
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
		Config::MapSettings::GridPosition searchPosition{ targetPos };
		int searchDistance{ distanceGrid[searchPosition.y][searchPosition.x] };

		// 距離が0(目的地と現在地が等しい)とき
		if (searchDistance == 0)
		{
			// 空のパスを返す
			return path;
		}

		// 事前にマップデータを取得
		const std::vector<std::vector<int>>& mapData{ FieldMap::GetInstance().getMapData() };

		// 目的地から始点（距離 0）まで探索
		while (searchDistance != 0)
		{
			// 現在地をパスに格納
			path.push_back(searchPosition);

			// 範囲for文で、隣接4マスの始点からの距離を評価
			for (const auto& offset : Config::MapSettings::GridOffset)
			{
				// 次の探索目標を設定
				const Config::MapSettings::GridPosition nextPosition{
					searchPosition.x + offset.x,
					searchPosition.y + offset.y
				};

				// マップの境界チェック
				if (nextPosition.x < 0 || nextPosition.x >= Config::MapSettings::MapWidth ||
					nextPosition.y < 0 || nextPosition.y >= Config::MapSettings::MapHeight)
				{
					continue;
				}

				// 次のマスの距離を参照
				const int nextDistance{ distanceGrid[nextPosition.y][nextPosition.x] };

				// 現在地より遠くへは行かない
				if (searchDistance < nextDistance)
				{
					continue;
				}

				// mapDataのint型をTileTypeに変換
				const Config::MapSettings::TileType searchTileType{ static_cast<Config::MapSettings::TileType>(mapData[searchPosition.y][searchPosition.x]) };

				// 侵入コストを対応表から取得
				const int accessCost{ Config::MapSettings::TileAccessCost.at(searchTileType) };

				// 隣接するマスの距離を計算し、現在地から侵入コスト分小さければ、そこを通過したことにする
				if (nextDistance == searchDistance - accessCost)
				{
					searchPosition = nextPosition;
					searchDistance = nextDistance;
					break;
				}
			}
		}

		// パスは目的地から始点になっているため、反転して始点から目的地にする
		std::reverse(path.begin(), path.end());

		// 完成した経路を返す
		return path;
	}
};