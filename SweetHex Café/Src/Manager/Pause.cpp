#include <DxLib.h>
#include "Pause.h"
#include "InputController.h"
#include "SceneManager.h"
#include "../Application.h"

Pause::Pause(void)
{
}

Pause::~Pause(void)
{
}

void Pause::Init(void)
{
	isPause_ = false;
	ChangePause(PAUSE::CONTINUE);
}

void Pause::Update(void)
{
	// ポーズメニューの状態ごとの更新
	switch (pause_)
	{
	case PAUSE::CONTINUE:
		UpdateContinue();
		break;
	case PAUSE::RETURN_TITLE:
		UpdateReturnTitle();
		break;
	case PAUSE::EXIT:
		UpdateExit();
		break;
	}

	// ポーズメニューの選択
	SelectPause();
}

void Pause::Draw(void)
{
	// 画面を暗くする
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 180);
	DrawBox(0, 0, Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	// ポーズメニューの状態ごとの描画(デバッグ)
	switch (pause_)
	{
	case PAUSE::CONTINUE:
		DrawContinue();
		break;
	case PAUSE::RETURN_TITLE:
		DrawReturnTitle();
		break;
	case PAUSE::EXIT:
		DrawExit();
		break;
	}
}

void Pause::Release(void)
{
}

bool Pause::IsPause(void)
{
	return isPause_;
}

void Pause::SetPause(bool pause)
{
	isPause_ = pause;
}

void Pause::ChangePause(PAUSE pause)
{
	pause_ = pause;
}

void Pause::SelectPause(void)
{
	InputController& ins = InputController::GetInstance();

	// 上キーでポーズメニューの選択
	if (ins.IsSelectUp())
	{
		ChangePause(static_cast<PAUSE>(static_cast<int>(pause_) - 1));
	}

	// メニューの範囲外に行かないようにする
	if(static_cast<int>(pause_) <= 0)
	{
		ChangePause(PAUSE::CONTINUE);
	}

	//	 下キーでポーズメニューの選択
	if (ins.IsSelectDown())
	{
		ChangePause(static_cast<PAUSE>(static_cast<int>(pause_) + 1));
	}

	// メニューの範囲外に行かないようにする
	if(static_cast<int>(pause_) >= static_cast<int>(PAUSE::MAX))
	{
		ChangePause(PAUSE::EXIT);
	}

	if (ins.IsPause())
	{
		// ポーズ解除
		SetPause(false);
	}

}

void Pause::UpdateContinue(void)
{
	InputController& ins = InputController::GetInstance();

	if (ins.IsDecide())
	{
		// ポーズ解除
		SetPause(false);
	}
}

void Pause::UpdateReturnTitle(void)
{
	InputController& ins = InputController::GetInstance();

	if (ins.IsDecide())
	{
		// タイトルに戻る
		SceneManager::GetInstance().ChangeScene(
			SceneManager::SCENE_ID::TITLE);
	}
}

void Pause::UpdateExit(void)
{
	InputController& ins = InputController::GetInstance();

	if (ins.IsDecide())
	{
		// ゲーム終了
		Application::GetInstance().SetEnd(true);
	}
}

void Pause::DrawContinue(void)
{
	DrawString(450, 300, "CONTINUE", 0x00ff00);
	DrawString(450, 320, "RETURN TITLE", 0xffffff);
	DrawString(450, 340, "QUIT GAME", 0xffffff);

}

void Pause::DrawReturnTitle(void)
{
	DrawString(450, 300, "CONTINUE", 0xffffff);
	DrawString(450, 320, "RETURN TITLE", 0x00ff00);
	DrawString(450, 340, "QUIT GAME", 0xffffff);
}

void Pause::DrawExit(void)
{
	DrawString(450, 300, "CONTINUE", 0xffffff);
	DrawString(450, 320, "RETURN TITLE", 0xffffff);
	DrawString(450, 340, "QUIT GAME", 0x00ff00);
}
