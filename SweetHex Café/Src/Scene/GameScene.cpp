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
	player_->Draw();
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
	timer_->Update();

	Collision();

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

void GameScene::Collision(void)
{
	float speed = player_->GetSpeed();

	if (speed > 0.0f)
	{
		// ステージブロックとプレイヤーの衝突
		VECTOR playerPos = player_->GetPos();

		// 向き
		VECTOR dir = player_->GetDir();

		// 前方座標
		const float COLLISION_OFFSET = 15.0f;
		const float COLLISION_HEIGHT = 10.0f;

		// 前方方向を計算
		VECTOR forwardPos = VAdd(playerPos, VScale(dir, COLLISION_OFFSET));

		// 線分の始点と終点
		VECTOR startPos = playerPos;
		VECTOR endPos = forwardPos;

		startPos.y = endPos.y = COLLISION_HEIGHT;

		// ステージとの衝突判定
		MV1_COLL_RESULT_POLY result;

		if (blockManager_->IsCollisionLine(startPos, endPos, &result))
		{
			// プレイヤーに衝突座標を渡す
			player_->CollisionStage(result.HitPosition);
		}
	}
}
