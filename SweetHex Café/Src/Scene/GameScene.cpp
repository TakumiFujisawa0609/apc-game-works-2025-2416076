#include <Dxlib.h>
#include "../Application.h"
#include "../Utility/Utility.h"
#include "../Manager/SceneManager.h"
#include "../Manager/InputManager.h"
#include "../Manager/InputController.h"
#include "../Object/Grid.h"
#include "../Object/Stage/BlockManager.h"
#include "../Object/Player.h"
#include "../Manager/Pause.h"
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

	ChangeState(STATE::GAME);

}

void GameScene::Update(void)
{
	InputController& ins = InputController::GetInstance();

	switch (state_)
	{
	case STATE::GAME:
		blockManager_->Update();
		grid_->Update();
		player_->Update();

		// ポーズメニューへ
		if(ins.IsPause())
		{
			pause_->SetPause(true);
			ChangeState(STATE::PAUSE);
		}

		break;
	case STATE::PAUSE:
		pause_->Update();

		if(!pause_->IsPause())
		{
			ChangeState(STATE::GAME);
		}

		break;
	}

}

void GameScene::Draw(void)
{
	grid_->Draw();
	blockManager_->Draw();
	player_->Draw();

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
}

void GameScene::ChangeState(STATE state)
{
	state_ = state;
}
