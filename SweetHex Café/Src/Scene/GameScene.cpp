#include <Dxlib.h>
#include "../Application.h"
#include "../Utility/Utility.h"

#include "../Manager/SceneManager.h"
#include "../Manager/InputManager.h"
#include "../Manager/InputController.h"
#include "../Manager/Pause.h"

#include "../Object/Grid.h"
#include "../Object/Stage/BlockManager.h"
#include "../Object/Player.h"
#include "../Object/Timer.h"
#include "../Object/Enemy/EnemyManager.h"
#include "../Object/Enemy/EnemyBase.h"

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

	player_ = new Player();
	player_->Init();

	pause_ = new Pause();
	pause_->Init();

	timer_ = new Timer();
	timer_->Init();

	enemyManager_ = new EnemyManager(player_);
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
	enemyManager_->Update();
	timer_->Update();
	player_->Update();

	Collision();
	CollisionEnemy();


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

void GameScene::Collision(void)
{
	// 進めるかどうかをチェックする
	player_->ProcessMove(blockManager_);
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


		// 敵とプレイヤーの衝突判定条件を満たしたら、
		if (Utility::IsHitSpheres(playerPos, Player::COLLISION_RADIUS,
			enemy->GetPos(), enemy->GetRadius()))
		{
			VECTOR enemyDir = enemy->GetDir();
			//player_->KnockBack(enemyDir, 20.0f);
			player_->Damage(1);
		}
	}
}