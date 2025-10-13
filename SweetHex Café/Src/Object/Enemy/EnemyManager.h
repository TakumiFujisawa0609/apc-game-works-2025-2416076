#pragma once
#include <vector>

class EnemyBase;
class Player;
class ItemManager;

class EnemyManager
{
public:

	static constexpr int SPAWN_INTERVEL_SLIME = 300;

	// 敵の最大数
	static constexpr int MAX_ENEMYS = 5;

	EnemyManager(Player* player, ItemManager* item);
	~EnemyManager();

	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);

	std::vector<EnemyBase*> GetEnemys(void);

	void CheckCollision(BlockManager* block);

private:
	// エネミー用のモデルハンドルID
	std::vector<int> enemyModelIds_;

	// エネミー
	std::vector<EnemyBase*> enemys_;

	Player* player_;
	ItemManager* item_;

	int cntSpawn_;
};