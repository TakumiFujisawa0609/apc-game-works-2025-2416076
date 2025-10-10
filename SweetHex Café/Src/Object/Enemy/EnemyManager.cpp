#include "../../Application.h"

#include "EnemyBase.h"
#include "EnemySlime.h"

#include "EnemyManager.h"

EnemyManager::EnemyManager(Player* player)
{
	player_ = player;
}

EnemyManager::~EnemyManager()
{
}

void EnemyManager::Init(void)
{
	cntSpawn_ = 0;

	enemyModelIds_.emplace_back(
		MV1LoadModel((Application::PATH_MODEL + "Enemy/Slime.mv1").c_str()));
}

void EnemyManager::Update(void)
{
	// 出現カウンタ
	cntSpawn_++;

	if (cntSpawn_ >= SPAWN_INTERVEL_SLIME)
	{
		EnemyBase* newEnemy = new EnemySlime();

		newEnemy->Init(EnemyBase::TYPE::SLIME, enemyModelIds_[0], player_);

		// リストに追加
		enemys_.emplace_back(newEnemy);

		// カウンターをリセット
		cntSpawn_ = 0;
	}

	for (EnemyBase* enemy : enemys_)
	{
		enemy->Update();
	}
}

void EnemyManager::Draw(void)
{
	for (EnemyBase* enemy : enemys_)
	{
		enemy->Draw();
	}
}

void EnemyManager::Release(void)
{
	for (EnemyBase* enemy : enemys_)
	{
		enemy->Release();
		delete enemy;
	}

	for (int id : enemyModelIds_)
	{
		MV1DeleteModel(id);
	}
}

std::vector<EnemyBase*> EnemyManager::GetEnemys(void)
{
	return enemys_;
}

void EnemyManager::MoveForward(BlockManager* block)
{
	for (EnemyBase* enemy : enemys_)
	{
		enemy->MoveForward(block);
	}
}