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
	isScale_(false),
	scoreImg_(-1),
	alpha_(0.0f),
	fontHandle_(-1),
	rank_("")
{
}

ResultScene::~ResultScene(void)
{
}

void ResultScene::Init(void)
{
	pos_ = { Application::SCREEN_SIZE_X / 2.0f, Application::SCREEN_SIZE_Y / 2.0f - 800.0f, 100.0f };
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

	alpha_ = 0.0f;

	rank_ = "";

	SoundManager::GetInstance()->Play(SoundManager::SE::REGISTER);
}

void ResultScene::Load(void)
{
	// モデルのハンドルID
	modelId_ = MV1LoadModel((Application::PATH_MODEL + "Player/Chef_Hat.mv1").c_str());
	animationController_ = new AnimationController(modelId_);

	gameClearImg_= LoadGraph((Application::PATH_IMAGE + "gameClear.png").c_str());
	spaceKeyImg_ = LoadGraph((Application::PATH_IMAGE + "spaceKey.png").c_str());
	aButtonImg_ = LoadGraph((Application::PATH_IMAGE + "aButton.png").c_str());

	scoreImg_ = LoadGraph((Application::PATH_IMAGE + "clear.png").c_str());

	fontHandle_ = CreateFontToHandle("HGSoeiKakupoptai", 24, -1);
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
	animationController_->Update();

	// 入力デバイス判定
	if (GetJoypadNum() == 0)
	{
		pushImg_ = spaceKeyImg_;
	}
	else
	{
		pushImg_ = aButtonImg_;
	}

	// アニメーション
	if (scale_ >= 1.5f)
	{
		isScale_ = true;
	}
	else if (scale_ <= 1.0f)
	{
		isScale_ = false;
	}

	// 拡大縮小処理
	if (isScale_)
	{
		scale_ -= 0.01f;
	}
	else
	{
		scale_ += 0.01f;
	}

	// フェイドイン処理
	if (alpha_ < 255.0f)
	{
		alpha_ += 1.5f;
	}

	// ランク判定
	if(SystemManager::GetInstance().GetScore() >= 2000)
	{
		rank_ = "S";
	}
	else if(SystemManager::GetInstance().GetScore() >= 1500)
	{
		rank_ = "A";
	}
	else if(SystemManager::GetInstance().GetScore() >= 1000)
	{
		rank_ = "B";
	}
	else
	{
		rank_ = "C";
	}

	if (InputController::GetInstance().IsDecide())
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


	SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(alpha_));

	DrawRotaGraph(
		Application::SCREEN_SIZE_X / 2 - 200,
		Application::SCREEN_SIZE_Y / 2,
		1.0, 0.0,
		scoreImg_, true);

	DrawFormatStringToHandle(
		Application::SCREEN_SIZE_X / 2 - 280,
		Application::SCREEN_SIZE_Y / 2 - 40, 0x000000,
		fontHandle_, "金額　：%d",SystemManager::GetInstance().GetScore());

	DrawFormatStringToHandle(
		Application::SCREEN_SIZE_X / 2 - 280,
		Application::SCREEN_SIZE_Y / 2 + 20, 0x000000,
		fontHandle_, "ランク：%s", rank_);

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
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

	DeleteGraph(scoreImg_);

	DeleteFontToHandle(fontHandle_);

	SoundManager::GetInstance()->StopSound();
}