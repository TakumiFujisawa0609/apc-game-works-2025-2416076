#include "Player.h"
#include "../Application.h"
#include "../Utility/Utility.h"
#include "../Manager/InputManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/Camera.h"
#include "Common/AnimationController.h"

Player::Player(void)
{
}

Player::~Player(void)
{
}

void Player::Init(void)
{
	// モデルのハンドルID
	modelId_ = MV1LoadModel((Application::PATH_MODEL + "Player/Chef_Hat.mv1").c_str());

	// 色の調整(自己発光)
	//MV1SetMaterialEmiColor(modelId_, 0, COLOR_EMI_DEFAULT);

	// 大きさ設定
	scales_ = SCALES;
	MV1SetScale(modelId_, scales_);

	// 位置設定
	pos_ = DEFAULT_POS;
	MV1SetPosition(modelId_, pos_);

	// モデルの角度
	angles_ = { 0.0f, 0.0f, 0.0f };
	MV1SetRotationXYZ(modelId_, angles_);

	animController_ = new AnimationController(modelId_);

	for (int i = 0; i < static_cast<int>(ANIM_TYPE::MAX); i++)
	{
		animController_->AddInFbx(i, 30.0f, i);
	}

	animController_->Play(static_cast<int>(ANIM_TYPE::IDLE));

	SceneManager::GetInstance().GetCamera()->SetFollow(this);

	hp_ = MAX_HP;
}

void Player::Update(void)
{
	ProcessMove();

	animController_->Update();
}

void Player::Draw(void)
{
	MV1DrawModel(modelId_);
}

void Player::Release(void)
{
	MV1DeleteModel(modelId_);

	animController_->Release();
	delete animController_;
}

VECTOR Player::GetPos(void) const
{
	return pos_;
}

void Player::ProcessMove(void)
{
	InputManager& ins = InputManager::GetInstance();

	VECTOR moveDir = Utility::VECTOR_ZERO;

	// キーが押されたら、その方向に移動する
	if (ins.IsNew(KEY_INPUT_W)) { moveDir = Utility::DIR_F; }
	if (ins.IsNew(KEY_INPUT_S)) { moveDir = Utility::DIR_B; }
	if (ins.IsNew(KEY_INPUT_A)) { moveDir = Utility::DIR_L; }
	if (ins.IsNew(KEY_INPUT_D)) { moveDir = Utility::DIR_R; }

	if (Utility::EqualsVZero(moveDir))
	{
		animController_->Play(static_cast<int>(ANIM_TYPE::IDLE));
	}
	else
	{
		moveDir_ = moveDir;

		float speed = MOVE_SPEED;

		if (ins.IsNew(KEY_INPUT_LSHIFT)) { speed *= 3.0f; }

		// 移動量を計算する(方向×スピード)
		VECTOR movePow = VScale(moveDir, speed);

		// 移動処理(座標+移動量)
		pos_ = VAdd(pos_, movePow);
		MV1SetPosition(modelId_, pos_);


		// 方向から角度を設定
		angles_.y = atan2(moveDir.x, moveDir.z);

		// モデルの向きが正の負の方向を向いているので、補正する
		angles_.y += Utility::Deg2RadF(180.0f);

		MV1SetRotationXYZ(modelId_, angles_);

		if (ins.IsNew(KEY_INPUT_LSHIFT))
		{
			animController_->Play(static_cast<int>(ANIM_TYPE::RUN));
		}
		else
		{
			animController_->Play(static_cast<int>(ANIM_TYPE::WALK));
		}
	}
}