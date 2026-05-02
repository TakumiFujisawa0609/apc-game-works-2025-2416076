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
	imgStart_ = -1;
	imgBackground_ = -1;
}

TitleScene::~TitleScene(void)
{
}

void TitleScene::Init(void)
{
	SystemManager::GetInstance().ResetGame();

	ChangeState(STATE::START);

	backgroundSpeed_ = 0.5f;

	idleFrameCount_ = 0;


	SoundManager::GetInstance()->Play(SoundManager::BGM::GAME);
}

void TitleScene::Load(void)
{
	imgTitle_ = LoadGraph((Application::PATH_IMAGE + "Title.png").c_str());
	imgStart_ = LoadGraph((Application::PATH_IMAGE + "start.png").c_str());
	imgQuit_ = LoadGraph((Application::PATH_IMAGE + "quit.png").c_str());
	imgBackground_ = LoadGraph((Application::PATH_IMAGE + "background.png").c_str());
}

void TitleScene::LoadEnd(void)
{
	Init();

	imgBackground2_ = imgBackground_;
}

void TitleScene::Update(void)
{
	bool hasInput = false;

	if (CheckHitKeyAll() != 0)
	{
		hasInput = true;
	}

	if (hasInput)
	{
		idleFrameCount_ = 0;
	}
	else
	{
		idleFrameCount_++;
	}

	if (idleFrameCount_ > 60 * 20) {
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::MOVIE);
		return; // ここで処理を抜けて動画専用の更新にする
	}

	backgroundPos_.x -= backgroundSpeed_;

	if (backgroundPos_.x <= -Application::SCREEN_SIZE_X)
	{
		backgroundPos_.x = 0.0f;
	}


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
	DrawGraphF(backgroundPos_.x, 0.0f, imgBackground_, true);
	DrawGraphF(backgroundPos_.x + 1020, 0.0f, imgBackground2_, true);

	DrawRotaGraph(
		Application::SCREEN_SIZE_X / 2,
		Application::SCREEN_SIZE_Y / 2 - 50,
		1.0, 0.0, imgTitle_, true);

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

}

void TitleScene::Release(void)
{
	DeleteGraph(imgBackground_);
	DeleteGraph(imgTitle_);
	DeleteGraph(imgStart_);
	DeleteGraph(imgQuit_);
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
		SoundManager::GetInstance()->Play(SoundManager::SE::ENTRY);
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
	DrawRotaGraph(
		Application::SCREEN_SIZE_X / 2,
		Application::SCREEN_SIZE_Y / 2 + 150,
		1.0, 0.0, imgStart_, true);

	DrawRotaGraph(
		Application::SCREEN_SIZE_X / 2,
		Application::SCREEN_SIZE_Y / 2 + 200,
		0.5, 0.0, imgQuit_, true);
}

void TitleScene::DrawExit(void)
{
	DrawRotaGraph(
		Application::SCREEN_SIZE_X / 2,
		Application::SCREEN_SIZE_Y / 2 + 150,
		0.5, 0.0, imgStart_, true);

	DrawRotaGraph(
		Application::SCREEN_SIZE_X / 2,
		Application::SCREEN_SIZE_Y / 2 + 200,
		1.0, 0.0, imgQuit_, true);
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