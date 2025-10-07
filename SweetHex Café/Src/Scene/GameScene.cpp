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
	// 進めるかどうかをチェックする
	player_->ProcessMove(blockManager_);
}
