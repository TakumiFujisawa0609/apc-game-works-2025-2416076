#include <DxLib.h>

#include "./../Application.h"
#include "../Manager/InputController.h"
#include "../Manager/SystemManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/SoundManager/SoundManager.h"

#include "../Utility/Utility.h"
#include "../Utility/MatrixUtility.h"

#include "../Object/Common/AnimationController.h"

#include "ResultScene.h"

ResultScene::ResultScene(void)
	:
	SceneBase(),
	animationController_(nullptr),
	modelId_(-1),
	pos_{ 0.0f,0.0f,0.0f },
	scales_{ 1.0f,1.0f,1.0f },
	localAngles_{ 0.0f,0.0f,0.0f },
	angles_{ 0.0f,0.0f,0.0f },
	gameClearImg_(-1),
	pushImg_(-1),
	spaceKeyImg_(-1),
	aButtonImg_(-1),
	scale_(1.0f),
	isScale_(false)
{
}

ResultScene::~ResultScene(void)
{
}

void ResultScene::Init(void)
{
	pos_ = { Application::SCREEN_SIZE_X / 2.0f - 100.0f, Application::SCREEN_SIZE_Y / 2.0f - 400.0f, -200.0f };
	MV1SetPosition(modelId_, pos_);

	scales_ = { 2.0f,2.0f,2.0f };
	MV1SetScale(modelId_, scales_);

	angles_ = Utility::VECTOR_ZERO;
	localAngles_ = { Utility::Deg2RadF(45.0f), Utility::Deg2RadF(15.0f), Utility::Deg2RadF(15.0f) };
	// 行列の合成(子, 親と指定すると親⇒子の順に適用される)
	MATRIX mat = MatrixUtility::Multiplication(localAngles_, angles_);

	// 回転行列をモデルに反映
	MV1SetRotationMatrix(modelId_, mat);

	animationController_->AddInFbx(14, 30.0f, 14);
	animationController_->Play(14,true);

	pushImg_ = spaceKeyImg_;

	isScale_ = false;
}

void ResultScene::Load(void)
{
	// モデルのハンドルID
	modelId_ = MV1LoadModel((Application::PATH_MODEL + "Player/Chef_Hat.mv1").c_str());
	animationController_ = new AnimationController(modelId_);

	gameClearImg_= LoadGraph((Application::PATH_IMAGE + "gameClear.png").c_str());
	spaceKeyImg_ = LoadGraph((Application::PATH_IMAGE + "spaceKey.png").c_str());
	aButtonImg_ = LoadGraph((Application::PATH_IMAGE + "aButton.png").c_str());
}

void ResultScene::LoadEnd(void)
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

void ResultScene::Update(void)
{
	InputController& ins = InputController::GetInstance();

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

	if (ins.IsDecide())
	{
		// シーンをタイトルに変更
		SceneManager::GetInstance().ChangeScene(
			SceneManager::SCENE_ID::TITLE);
	}
}

void ResultScene::Draw(void)
{
	MV1DrawModel(modelId_);

	DrawRotaGraph(
		Application::SCREEN_SIZE_X / 2,
		100,
		1.0, 0.0,
		gameClearImg_, true);

	DrawRotaGraph(
		Application::SCREEN_SIZE_X / 2,
		Application::SCREEN_SIZE_Y - 100,
		static_cast<double>(scale_), 0.0,
		pushImg_, true);

	//DrawFormatString(Application::SCREEN_SIZE_X / 2, Application::SCREEN_SIZE_Y / 2, 0x000000, "金額 : %d", SystemManager::GetInstance().GetScore());
}

void ResultScene::Release(void)
{
	MV1DeleteModel(modelId_);

	animationController_->Release();
	delete animationController_;
	animationController_ = nullptr;

	DeleteGraph(gameClearImg_);
	DeleteGraph(spaceKeyImg_);
	DeleteGraph(aButtonImg_);
	pushImg_ = -1;

	SoundManager::GetInstance()->StopSound();
}