#include <DxLib.h>

#include "./../Application.h"
#include "../Manager/InputController.h"
#include "../Manager/SceneManager.h"
#include "../Manager/SoundManager/SoundManager.h"

#include "GameOverScene.h"

GameOverScene::GameOverScene(void)
	:
	SceneBase(),
	gameOverImg_(-1),
	pushImg_(-1),
	spaceKeyImg_(-1),
	aButtonImg_(-1),
	scale_(1.0f),
	isScale_(false)
{
}

GameOverScene::~GameOverScene(void)
{
}

void GameOverScene::Init(void)
{
	pushImg_ = spaceKeyImg_;

	isScale_ = false;
}

void GameOverScene::Load(void)
{
	gameOverImg_ = LoadGraph((Application::PATH_IMAGE + "gameOver.png").c_str());
	spaceKeyImg_ = LoadGraph((Application::PATH_IMAGE + "spaceKey.png").c_str());
	aButtonImg_ = LoadGraph((Application::PATH_IMAGE + "aButton.png").c_str());
}

void GameOverScene::LoadEnd(void)
{
	Init();

	if (GetJoypadNum() == 0)
	{
		pushImg_ = spaceKeyImg_;
	}
	else
	{
		pushImg_ = aButtonImg_;
	}
}

void GameOverScene::Update(void)
{

	if (scale_ >= 1.5f)
	{
		isScale_ = true;
	}
	else if (scale_ <= 1.0f)
	{
		isScale_ = false;
	}


	if (isScale_)
	{
		scale_ -= 0.01f;
	}
	else
	{
		scale_ += 0.01f;
	}

	if (GetJoypadNum() == 0)
	{
		pushImg_ = spaceKeyImg_;
	}
	else
	{
		pushImg_ = aButtonImg_;
	}

	if (InputController::GetInstance().IsDecide())
	{
		// シーンをタイトルに変更
		SceneManager::GetInstance().ChangeScene(
			SceneManager::SCENE_ID::TITLE);
	}
}

void GameOverScene::Draw(void)
{
	DrawRotaGraph(
		Application::SCREEN_SIZE_X / 2,
		100,
		1.0, 0.0,
		gameOverImg_, true);

	DrawRotaGraph(
		Application::SCREEN_SIZE_X / 2,
		Application::SCREEN_SIZE_Y - 100,
		static_cast<double>(scale_), 0.0,
		pushImg_, true);
}

void GameOverScene::Release(void)
{
	DeleteGraph(gameOverImg_);
	DeleteGraph(spaceKeyImg_);
	DeleteGraph(aButtonImg_);
	pushImg_ = -1;

	SoundManager::GetInstance()->StopSound();
}