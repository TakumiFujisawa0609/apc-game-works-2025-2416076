#pragma once
#include <vector>

class EnemyBase;
class Player;

class EnemyManager
{
public:

	static constexpr int SPAWN_INTERVEL_SLIME = 300;

	EnemyManager(Player* player);
	~EnemyManager();

	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);

	std::vector<EnemyBase*> GetEnemys(void);

	void MoveForward(BlockManager* block);

private:
	// エネミー用のモデルハンドルID
	std::vector<int> enemyModelIds_;

	// エネミー
	std::vector<EnemyBase*> enemys_;

	Player* player_;

	int cntSpawn_;
};