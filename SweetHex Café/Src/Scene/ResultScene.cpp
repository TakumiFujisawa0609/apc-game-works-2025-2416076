#include <DxLib.h>

#include "./../Application.h"
#include "../Manager/InputController.h"
#include "../Manager/SystemManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/SoundManager/SoundManager.h"

#include "ResultScene.h"

ResultScene::ResultScene(void)
{
}

ResultScene::~ResultScene(void)
{
}

void ResultScene::Init(void)
{
}

void ResultScene::Update(void)
{
	InputController& ins = InputController::GetInstance();

	if (ins.IsDecide())
	{
		// シーンをタイトルに変更
		SceneManager::GetInstance().ChangeScene(
			SceneManager::SCENE_ID::TITLE);
	}
}

void ResultScene::Draw(void)
{
	DrawFormatString(Application::SCREEN_SIZE_X / 2, Application::SCREEN_SIZE_Y / 2, 0x000000, "score : %d", SystemManager::GetInstance().GetScore());

	DrawString(0, 0, "ResultScene", 0xffffff);

	DrawString(450, 500, "PRESS SPACE KEY", 0xffffff);
}

void ResultScene::Release(void)
{
	SoundManager::GetInstance()->StopSound();
}