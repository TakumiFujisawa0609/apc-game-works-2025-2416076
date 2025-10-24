#include <DxLib.h>

#include "../Application.h"

#include "../Manager/SceneManager.h"
#include "../Manager/InputController.h"
#include "../Manager/InputManager.h"
#include "../Manager/SystemManager.h"
#include "../Manager/SoundManager/SoundManager.h"

#include "../Utility/Utility.h"

#include "TitleScene.h"

TitleScene::TitleScene(void)
{
	imgTitle_ = -1;
}

TitleScene::~TitleScene(void)
{
}

void TitleScene::Init(void)
{
	imgTitle_ = LoadGraph((Application::PATH_IMAGE + "Title.png").c_str());

	SystemManager::GetInstance().ResetGame();

	ChangeState(STATE::START);

	SoundManager::GetInstance()->Play(SoundManager::BGM::GAME);
}

void TitleScene::Update(void)
{
	// 状態ごとの更新
	switch (state_)
	{
	case STATE::START:
		UpdateStart();
		break;
	case STATE::EXIT:
		UpdateExit();
		break;
	}

	// タイトルメニュー選択
	SelectTitleMenu();
}

void TitleScene::Draw(void)
{
	DrawRotaGraph(
		Application::SCREEN_SIZE_X / 2,
		Application::SCREEN_SIZE_Y / 2,
		1.0f, 0.0, imgTitle_, true);

	// 状態ごとの描画
	switch (state_)
	{
	case STATE::START:
		DrawStart();
		break;
	case STATE::EXIT:
		DrawExit();
		break;
	}

	DrawString(
		Application::SCREEN_SIZE_X / 2 - 80,
		Application::SCREEN_SIZE_Y / 2,
		"SweetHex Cafe", 0x000000);

#ifdef _DEBUG
	DrawString(0, 0, "TitleScene", 0xffffff);
#endif // _DEBUG

}

void TitleScene::Release(void)
{
	DeleteGraph(imgTitle_);
}

void TitleScene::ChangeState(STATE state)
{
	state_ = state;
}

void TitleScene::UpdateStart(void)
{
	InputController& ins = InputController::GetInstance();

	// スペースキーでゲーム開始
	if (ins.IsDecide())
	{
		SceneManager::GetInstance().ChangeScene(
			SceneManager::SCENE_ID::GAME);
	}
}

void TitleScene::UpdateExit(void)
{
	InputController& ins = InputController::GetInstance();

	// スペースキーで終了
	if (ins.IsDecide())
	{
		Application::GetInstance().SetEnd(true);
	}
}

void TitleScene::DrawStart(void)
{
	DrawString(450, 500, "NEW GAME", 0x00ff00);
	DrawString(450, 520, "QUIT GAME", 0xffffff);
}

void TitleScene::DrawExit(void)
{
	DrawString(450, 500, "NEW GAME", 0xffffff);
	DrawString(450, 520, "QUIT GAME", 0x00ff00);
}

void TitleScene::SelectTitleMenu(void)
{
	InputController& ins = InputController::GetInstance();
	VECTOR dir = Utility::VECTOR_ZERO;


	// キーボード
	if (GetJoypadNum() == 0)
	{
		// 上キーでポーズメニューの選択
		if (ins.IsSelectUp())
		{
			ChangeState(static_cast<STATE>(static_cast<int>(state_) - 1));
		}

		//	 下キーでポーズメニューの選択
		if (ins.IsSelectDown())
		{
			ChangeState(static_cast<STATE>(static_cast<int>(state_) + 1));
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
			ChangeState(static_cast<STATE>(static_cast<int>(state_) - 1));
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
			ChangeState(static_cast<STATE>(static_cast<int>(state_) + 1));
		}
		else if (stickDownPressed && dir.z < -RESET_ZONE)
		{
			// ニュートラルに戻したらリセット
			stickDownPressed = false;
		}
	}


	if (static_cast<int>(state_) <= 0)
	{
		ChangeState(STATE::START);
	}

	if(static_cast<int>(state_) >= static_cast<int>(STATE::MAX) - 1)
	{
		ChangeState(STATE::EXIT);
	}
}