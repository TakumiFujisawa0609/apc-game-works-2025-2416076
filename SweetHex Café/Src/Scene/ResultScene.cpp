#include <DxLib.h>
#include "../Manager/InputController.h"
#include "../Manager/SystemManager.h"
#include "../Manager/SceneManager.h"


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
	DrawFormatString(10, 80, 0x000000, "score : %d", SystemManager::GetInstance().GetScore());

#ifdef _DEBUG
	DrawString(0, 0, "ResultScene", 0xffffff);

	DrawString(450, 500, "PRESS SPACE KEY", 0xffffff);
#endif // _DEBUG
}

void ResultScene::Release(void)
{
}