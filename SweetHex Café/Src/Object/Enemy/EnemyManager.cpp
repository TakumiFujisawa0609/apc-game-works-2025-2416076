#include "../../Application.h"

#include "EnemyBase.h"
#include "EnemySlime.h"
#include "../Item/ItemManager.h"

#include "EnemyManager.h"

EnemyManager::EnemyManager(Player* player, ItemManager* item)
{
	player_ = player;
	item_ = item;
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
	if (enemys_.size() < MAX_ENEMYS)
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
	}

	// イテレータを取得
	auto it = enemys_.begin();

	// リストの最後までループ
	while (it != enemys_.end())
	{
		EnemyBase* enemy = *it;

		enemy->Update();

		// 死亡フラグをチェック
		if (!enemy->IsAlive())
		{
			VECTOR dropPos = enemy->GetPos();

			if (item_ != nullptr)
			{
				// 敵が死んだときアイテムをドロップ
				item_->CreateItem(ItemManager::ITEM_ID_DEFAULT, dropPos);
			}

			// 敵のRelease処理を呼び出す
			enemy->Release();
			// メモリを解放
			delete enemy;

			// リストから要素を削除し、次の要素のイテレータを取得
			it = enemys_.erase(it);
		}
		else
		{
			// 生存している場合は、次の要素に進む
			++it;
		}
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

void EnemyManager::CheckCollision(BlockManager* block)
{
	for (EnemyBase* enemy : enemys_)
	{
		enemy->CheckCollision(block);
	}
}