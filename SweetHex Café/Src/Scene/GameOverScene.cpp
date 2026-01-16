#include <DxLib.h>

#include "./../Application.h"
#include "../Manager/InputController.h"
#include "../Manager/SceneManager.h"
#include "../Manager/SoundManager/SoundManager.h"

#include "../Utility/Utility.h"
#include "../Utility/MatrixUtility.h"

#include "../Object/Common/AnimationController.h"

#include "GameOverScene.h"

GameOverScene::GameOverScene(void)
	:
	SceneBase(),
	animationController_(nullptr),
	modelId_(-1),
	pos_{ 0.0f,0.0f,0.0f },
	scales_{ 1.0f,1.0f,1.0f },
	localAngles_{ 0.0f,0.0f,0.0f },
	angles_{ 0.0f,0.0f,0.0f },
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
	pos_ = { Application::SCREEN_SIZE_X / 2.0f - 500.0f, Application::SCREEN_SIZE_Y / 2.0f - 700.0f, 100.0f };
	MV1SetPosition(modelId_, pos_);

	scales_ = { 2.0f,2.0f,2.0f };
	MV1SetScale(modelId_, scales_);

	angles_ = Utility::VECTOR_ZERO;
	localAngles_ = { Utility::Deg2RadF(45.0f), Utility::Deg2RadF(15.0f), Utility::Deg2RadF(15.0f) };
	// 行列の合成(子, 親と指定すると親⇒子の順に適用される)
	MATRIX mat = MatrixUtility::Multiplication(localAngles_, angles_);

	// 回転行列をモデルに反映
	MV1SetRotationMatrix(modelId_, mat);

	animationController_->AddInFbx(0, 20.0f, 0);
	animationController_->Play(0, false);

	pushImg_ = spaceKeyImg_;

	isScale_ = false;
}

void GameOverScene::Load(void)
{
	// モデルのハンドルID
	modelId_ = MV1LoadModel((Application::PATH_MODEL + "Player/Chef_Hat.mv1").c_str());
	animationController_ = new AnimationController(modelId_);

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
	animationController_->Update();

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
	DrawBox(0, 0, Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, 0x000b1c, true);

	MV1DrawModel(modelId_);

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
	MV1DeleteModel(modelId_);

	animationController_->Release();
	delete animationController_;
	animationController_ = nullptr;

	DeleteGraph(gameOverImg_);
	DeleteGraph(spaceKeyImg_);
	DeleteGraph(aButtonImg_);
	pushImg_ = -1;

	SoundManager::GetInstance()->StopSound();
}