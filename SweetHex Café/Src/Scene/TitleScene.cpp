#include <DxLib.h>
#include "../Application.h"
#include "../Manager/SceneManager.h"
#include "../Manager/InputController.h"
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

	ChangeState(STATE::START);
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

	if (ins.IsSelectUp())
	{
		ChangeState(static_cast<STATE>(static_cast<int>(state_) - 1));
	}

	if (static_cast<int>(state_) <= 0)
	{
		ChangeState(STATE::START);
	}

	if (ins.IsSelectDown())
	{
		ChangeState(static_cast<STATE>(static_cast<int>(state_) + 1));
	}

	if(static_cast<int>(state_) >= static_cast<int>(STATE::MAX) - 1)
	{
		ChangeState(STATE::EXIT);
	}
}