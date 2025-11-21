#pragma once
#include <vector>

#include "EnemyBase.h"

class Player;
class ItemManager;
class Stage;
class OrderManager;

class EnemyManager
{
public:

	static constexpr int SPAWN_INTERVEL_SLIME = 120;

	// 敵の最大数
	static constexpr int MAX_ENEMYS = 5;

	EnemyManager(Player* player, ItemManager* item, Stage* stage, OrderManager* order);
	~EnemyManager();

	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);

	std::vector<EnemyBase*> GetEnemies(void);

private:
	// エネミー用のモデルハンドルID
	std::vector<int> enemyModelIds_;

	// エネミー
	std::vector<EnemyBase*> enemies_;

	Player* player_;
	Stage* stage_;
	ItemManager* item_;
	OrderManager* order_;

	int cntSpawn_;

	int nextPatternIndex_;

	// 注文番号
	int orderCounter_;

	EnemyBase::PATTERN UnusedPattern(void);
};