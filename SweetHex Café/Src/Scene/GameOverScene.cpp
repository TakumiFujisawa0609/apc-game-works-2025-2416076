#include <DxLib.h>
#include "../Manager/InputController.h"
#include "../Manager/SceneManager.h"
#include "GameOverScene.h"

GameOverScene::GameOverScene(void)
{
}

GameOverScene::~GameOverScene(void)
{
}

void GameOverScene::Init(void)
{
}

void GameOverScene::Update(void)
{
	InputController& ins = InputController::GetInstance();

	if (ins.IsDecide())
	{
		// シーンをタイトルに変更
		SceneManager::GetInstance().ChangeScene(
			SceneManager::SCENE_ID::TITLE);
	}
}

void GameOverScene::Draw(void)
{
	DrawString(0, 0, "GameOverScene", 0xffffff);

	DrawString(450, 500, "PRESS SPACE KEY", 0xffffff);
}

void GameOverScene::Release(void)
{
}