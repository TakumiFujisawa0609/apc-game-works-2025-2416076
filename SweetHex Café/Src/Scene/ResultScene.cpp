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
	angles_{ 0.0f,0.0f,0.0f }
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

}

void ResultScene::Load(void)
{
	// モデルのハンドルID
	modelId_ = MV1LoadModel((Application::PATH_MODEL + "Player/Chef_Hat.mv1").c_str());
	animationController_ = new AnimationController(modelId_);
}

void ResultScene::LoadEnd(void)
{
	Init();
}

void ResultScene::Update(void)
{
	InputController& ins = InputController::GetInstance();

	animationController_->Update();

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

	//DrawFormatString(Application::SCREEN_SIZE_X / 2, Application::SCREEN_SIZE_Y / 2, 0x000000, "金額 : %d", SystemManager::GetInstance().GetScore());

	//DrawString(450, 500, "PRESS SPACE KEY", 0xffffff);
}

void ResultScene::Release(void)
{
	MV1DeleteModel(modelId_);

	animationController_->Release();
	delete animationController_;
	animationController_ = nullptr;

	SoundManager::GetInstance()->StopSound();
}