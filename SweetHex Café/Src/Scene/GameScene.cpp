#include <Dxlib.h>
#include "../Application.h"
#include "../Utility/Utility.h"

#include "../Manager/SceneManager.h"
#include "../Manager/InputManager.h"
#include "../Manager/InputController.h"
#include "../Manager/SoundManager/SoundManager.h"
#include "../Manager/Pause.h"

#include "../Object/Grid.h"
#include "../Object/Stage/BlockManager.h"
#include "../Object/Player/Player.h"
#include "../Object/Timer.h"
#include "../Object/Enemy/EnemyManager.h"
#include "../Object/Enemy/EnemyBase.h"
#include "../Object/Weapon/WeaponBase.h"
#include "../Object/Item/ItemManager.h"

#include "GameScene.h"

GameScene::GameScene(void)
{
}

GameScene::~GameScene(void)
{
}

void GameScene::Init(void)
{
	grid_ = new Grid();
	grid_->Init();

	blockManager_ = new BlockManager();
	blockManager_->Init();

	pause_ = new Pause();
	pause_->Init();

	timer_ = new Timer();
	timer_->Init();

	item_ = new ItemManager();
	item_->Init();

	player_ = new Player(item_, blockManager_);
	player_->Init();

	enemyManager_ = new EnemyManager(player_, item_, blockManager_);
	enemyManager_->Init();

	ChangeState(STATE::GAME);

}

void GameScene::Update(void)
{
	switch (state_)
	{
	case STATE::GAME:
		UpdateGame();
		break;
	case STATE::PAUSE:
		UpdatePause();
		break;
	}
}

void GameScene::Draw(void)
{
	grid_->Draw();
	blockManager_->Draw();
	enemyManager_->Draw();
	player_->Draw(blockManager_);
	timer_->Draw();

	if (item_ != nullptr)
	{
		item_->Draw();
	}

	if (state_ == STATE::PAUSE)
	{
		pause_->Draw();
	}

#ifdef _DEBUG
	DrawString(0, 0, "GameScene", 0xffffff);
#endif // _DEBUG

}

void GameScene::Release(void)
{
	grid_->Release();
	delete grid_;

	blockManager_->Release();
	delete blockManager_;

	player_->Release();
	delete player_;

	enemyManager_->Release();
	delete enemyManager_;

	pause_->Release();
	delete pause_;

	timer_->Release();
	delete timer_;

	item_->Release();
	delete item_;
}

void GameScene::ChangeState(STATE state)
{
	state_ = state;
}

void GameScene::UpdateGame(void)
{
	InputController& ins = InputController::GetInstance();

	blockManager_->Update();
	grid_->Update();
	timer_->Update();

	// 当たり判定
	CollisionEnemy();
	CollisionWeapon();
	CollisionEnemy2Enemy();

	enemyManager_->Update();
	player_->Update();

	// ポーズメニューへ
	if (ins.IsPause())
	{
		pause_->SetPause(true);
		ChangeState(STATE::PAUSE);
	}

	// タイマーが0になったらクリアに遷移
	if (timer_->GetIsTimeUp())
	{
		SceneManager::GetInstance().ChangeScene(
			SceneManager::SCENE_ID::RESULT);
	}
}

void GameScene::UpdatePause(void)
{
	pause_->Update();

	if (!pause_->IsPause())
	{
		ChangeState(STATE::GAME);
	}
}

void GameScene::CollisionEnemy(void)
{
	// エネミーとプレイヤーの衝突判定
	VECTOR playerPos = player_->GetPos();
	// 敵の情報を取得
	std::vector<EnemyBase*> enemys = enemyManager_->GetEnemys();
	for (EnemyBase* enemy : enemys)
	{
		if (!enemy->IsCollisionState())
		{
			continue;
		}

		if (!player_->IsCollisionState())
		{
			continue;
		}

		VECTOR enemyPos = enemy->GetPos();

	
			// 敵とプレイヤーの衝突判定条件を満たしたら、
		if (Utility::IsHitSpheres(playerPos, Player::COLLISION_RADIUS,
			enemyPos, enemy->GetRadius()))
		{
			bool isMoveEnemy = !enemy->IsCollisionStage();
			bool isMovePlayer = !player_->IsCollisionStage();

			// 両方ともステージと衝突している場合は、押し出し処理をスキップ
			if (!isMoveEnemy && !isMovePlayer)
			{
				continue;
			}

			Utility::AdjustPositionCollision(playerPos, Player::COLLISION_RADIUS,
				enemyPos, enemy->GetRadius());

			if (isMoveEnemy)
			{
				enemy->SetPos(enemyPos);
			}

			if (isMovePlayer)
			{
				player_->SetPos(playerPos);
			}

			if (enemy->GetState() == EnemyBase::STATE::ATTACK)
			{
				if (!player_->IsInvincible())
				{
					player_->Damage(1);
				}
			}
		}

	}
}

void GameScene::CollisionWeapon(void)
{

	// 武器の情報
	WeaponBase* useWeapon = player_->GetUseWeapon();

	// 攻撃中（描画されている）なら
	if (useWeapon->IsAlive())
	{
		// 敵の情報を取得
		std::vector<EnemyBase*> enemys = enemyManager_->GetEnemys();
		for (EnemyBase* enemy : enemys)
		{
			if (!enemy->IsCollisionState())
			{
				continue;
			}

			// 敵と武器の当たり判定
			if (Utility::IsHitSpheres(useWeapon->GetPos(), useWeapon->GetCollisionRadius(),
				enemy->GetPos(), enemy->GetRadius()))
			{
				enemy->Damage(1);

				for (EnemyBase* enemy : enemyManager_->GetEnemys())
				{
					// 警戒状態で攻撃されていない子なら、追跡・攻撃処理へ
					if (enemy->IsNotice() && enemy->GetState() == EnemyBase::STATE::STANDBY)
					{
						enemy->ChangeState(EnemyBase::STATE::ATTACK);
					}
				}
			}
		}

		// 武器とブロックの当たり判定
		player_->CollisionWeapon(blockManager_);
	}
}

void GameScene::CollisionEnemy2Enemy(void)
{
	// 敵の情報を取得
	std::vector<EnemyBase*> enemies = enemyManager_->GetEnemys();

	// 敵同士の総当たり衝突判定と押し出し処理
	for (size_t i = 0; i < enemies.size(); ++i)
	{
		for (size_t j = i + 1; j < enemies.size(); ++j)
		{
			EnemyBase* enemyA = enemies[i];
			EnemyBase* enemyB = enemies[j];

			// 衝突していたら
			if (Utility::IsHitSpheres(enemyA->GetPos(), enemyA->GetRadius(),
				enemyB->GetPos(), enemyB->GetRadius()))
			{

				bool isMovableA = !enemyA->IsCollisionStage();
				bool isMovableB = !enemyB->IsCollisionStage();

				// 両方ともステージと衝突している場合は、押し出し処理をスキップ
				if (!isMovableA && !isMovableB)
				{
					continue;
				}

				// 敵の位置を一時的に格納
				VECTOR tempPosA = enemyA->GetPos();
				VECTOR tempPosB = enemyB->GetPos();

				// 位置を補正
				Utility::AdjustPositionCollision(tempPosA, enemyA->GetRadius(),
					tempPosB, enemyB->GetRadius());

				// 補正された位置を各オブジェクトに反映
				if (isMovableA) 
				{
					enemyA->SetPos(tempPosA);
				}

				if (isMovableB)
				{
					enemyB->SetPos(tempPosB);
				}
			}
		}
	}
}