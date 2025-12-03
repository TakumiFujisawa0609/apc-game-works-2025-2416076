#include <DxLib.h>

#include "../Application.h"

#include "InputController.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "SoundManager/SoundManager.h"

#include "../Utility/Utility.h"

#include "Pause.h"

Pause::Pause(void)
	:
	imgContinue_(-1),
	imgReturn_(-1),
	imgQuit_(-1)
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

void Pause::Load(void)
{
	imgContinue_ = LoadGraph((Application::PATH_IMAGE + "continue.png").c_str());
	imgReturn_ = LoadGraph((Application::PATH_IMAGE + "returnTitle.png").c_str());
	imgQuit_ = LoadGraph((Application::PATH_IMAGE + "quit.png").c_str());
}

void Pause::LoadEnd(void)
{
	Init();
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
	DeleteGraph(imgContinue_);
	DeleteGraph(imgReturn_);
	DeleteGraph(imgQuit_);
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
	VECTOR dir = Utility::VECTOR_ZERO;

	// キーボード
	if (GetJoypadNum() == 0)
	{
		// 上キーでポーズメニューの選択
		if (ins.IsSelectUp())
		{
			ChangePause(static_cast<PAUSE>(static_cast<int>(pause_) - 1));
		}

		//	 下キーでポーズメニューの選択
		if (ins.IsSelectDown())
		{
			ChangePause(static_cast<PAUSE>(static_cast<int>(pause_) + 1));
		}
	}
	// ゲームパッド
	else
	{

		InputManager::JOYPAD_IN_STATE padState =
			InputManager::GetInstance().GetJPadInputState(InputManager::JOYPAD_NO::PAD1);

		dir = InputManager::GetInstance().GetDirectionXZAKey(padState.AKeyLX, padState.AKeyLY);

		// スティックの上下入力を一度だけ反応させる
		static bool stickUpPressed = false;
		static bool stickDownPressed = false;
		const float THRESHOLD = 0.8f;
		const float RESET_ZONE = 0.1f;

		// 上方向
		if (!stickUpPressed && dir.z < THRESHOLD)
		{
			stickUpPressed = true;
			ChangePause(static_cast<PAUSE>(static_cast<int>(pause_) - 1));
		}
		else if (stickUpPressed && dir.z > RESET_ZONE)
		{
			// ニュートラルに戻したらリセット
			stickUpPressed = false; 
		}

		// 下方向
		if (!stickDownPressed && dir.z > -THRESHOLD)
		{
			stickDownPressed = true;
			ChangePause(static_cast<PAUSE>(static_cast<int>(pause_) + 1));
		}
		else if (stickDownPressed && dir.z < -RESET_ZONE)
		{
			// ニュートラルに戻したらリセット
			stickDownPressed = false;
		}
	}

	// メニューの範囲外に行かないようにする
	if(static_cast<int>(pause_) <= 0)
	{
		ChangePause(PAUSE::CONTINUE);
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
		SoundManager::GetInstance()->StopSound();
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
	DrawRotaGraph(
		Application::SCREEN_SIZE_X / 2,
		Application::SCREEN_SIZE_Y / 2 - 50,
		1.0, 0.0, imgContinue_, true);

	DrawRotaGraph(
		Application::SCREEN_SIZE_X / 2,
		Application::SCREEN_SIZE_Y / 2,
		0.5, 0.0, imgReturn_, true);

	DrawRotaGraph(
		Application::SCREEN_SIZE_X / 2,
		Application::SCREEN_SIZE_Y / 2 + 50,
		0.5, 0.0, imgQuit_, true);
}

void Pause::DrawReturnTitle(void)
{
	DrawRotaGraph(
		Application::SCREEN_SIZE_X / 2,
		Application::SCREEN_SIZE_Y / 2 - 50,
		0.5, 0.0, imgContinue_, true);

	DrawRotaGraph(
		Application::SCREEN_SIZE_X / 2,
		Application::SCREEN_SIZE_Y / 2,
		1.0, 0.0, imgReturn_, true);

	DrawRotaGraph(
		Application::SCREEN_SIZE_X / 2,
		Application::SCREEN_SIZE_Y / 2 + 50,
		0.5, 0.0, imgQuit_, true);
}

void Pause::DrawExit(void)
{
	DrawRotaGraph(
		Application::SCREEN_SIZE_X / 2,
		Application::SCREEN_SIZE_Y / 2 - 50,
		0.5, 0.0, imgContinue_, true);

	DrawRotaGraph(
		Application::SCREEN_SIZE_X / 2,
		Application::SCREEN_SIZE_Y / 2,
		0.5, 0.0, imgReturn_, true);

	DrawRotaGraph(
		Application::SCREEN_SIZE_X / 2,
		Application::SCREEN_SIZE_Y / 2 + 50,
		1.0, 0.0, imgQuit_, true);
}
