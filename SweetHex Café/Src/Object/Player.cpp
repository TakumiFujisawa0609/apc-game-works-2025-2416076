#include "Player.h"
#include "../Application.h"
#include "../Utility/Utility.h"
#include "../Manager/InputManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/InputController.h"
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

	ChangeState(STATE::STANDBY);
}

void Player::Update(void)
{
	switch (state_)
	{
	case Player::STATE::STANDBY:
		UpdateStandby();
		break;
	case Player::STATE::KNOCKBACK:
		UpdateKnockback();
		break;
	case Player::STATE::ATTACK:
		UpdateAttack();
		break;
	case Player::STATE::DEAD:
		UpdateDead();
		break;
	}

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

void Player::ChangeState(STATE state)
{
	state_ = state;

	switch (state_)
	{
	case Player::STATE::STANDBY:
		ChangeStandby();
		break;
	case Player::STATE::KNOCKBACK:
		ChangeKnockback();
		break;
	case Player::STATE::ATTACK:
		ChangeAttack();
		break;
	case Player::STATE::DEAD:
		ChangeDead();
		break;
	}
}

void Player::CollisionStage(VECTOR pos)
{
	// 衝突判定に指定座標に押し戻す
	pos_ = pos;
}

void Player::Damage(int damage)
{
	hp_ -= damage;
	if (hp_ < 0)
	{
		hp_ = 0;
	}

	if (hp_ <= 0)
	{
		ChangeState(STATE::DEAD);
	}
}

void Player::ProcessMove(void)
{
	InputController& ins = InputController::GetInstance();

	VECTOR moveDir = Utility::VECTOR_ZERO;

	// キーが押されたら、その方向に移動する
	if (ins.IsUp()) { moveDir = Utility::DIR_F; }
	if (ins.IsDown()) { moveDir = Utility::DIR_B; }
	if (ins.IsLeft()) { moveDir = Utility::DIR_L; }
	if (ins.IsRight()) { moveDir = Utility::DIR_R; }

	if (Utility::EqualsVZero(moveDir))
	{
		animController_->Play(static_cast<int>(ANIM_TYPE::IDLE));
	}
	else
	{
		moveDir_ = moveDir;

		float speed = MOVE_SPEED;

		if (ins.IsDash()) { speed = DASH_SPEED; }

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

		// アニメーションの切り替え
		if (ins.IsDash())
		{
			// ダッシュ時
			animController_->Play(static_cast<int>(ANIM_TYPE::RUN));
		}
		else
		{
			// 通常移動時
			animController_->Play(static_cast<int>(ANIM_TYPE::WALK));
		}
	}
}

void Player::ProcessAttack(void)
{
	InputController& ins = InputController::GetInstance();

	// 攻撃ボタンが押されたら
	if (ins.IsAttack())
	{
		// 攻撃状態にする
		ChangeState(STATE::ATTACK);
	}
}

void Player::ChangeStandby(void)
{
	animController_->Play(static_cast<int>(ANIM_TYPE::IDLE));
}

void Player::ChangeKnockback(void)
{
	animController_->Play(static_cast<int>(ANIM_TYPE::RECIEVE_HIT), false);
}

void Player::ChangeAttack(void)
{
	animController_->Play(static_cast<int>(ANIM_TYPE::PUNCH), false);
}

void Player::ChangeDead(void)
{
	animController_->Play(static_cast<int>(ANIM_TYPE::DEATH), false);
}

void Player::UpdateStandby(void)
{
	ProcessMove();

	ProcessAttack();
}

void Player::UpdateKnockback(void)
{
	cntKnockBack_++;

	if (cntKnockBack_ % TERM_BLINK == 0)
	{
		MV1SetMaterialDifColor(modelId_, 0, COLOR_DIF_DEFAULT);
	}
	else
	{
		MV1SetMaterialDifColor(modelId_, 0, COLOR_DIF_BLINK);
	}

	// アニメーションが終わったら、エンド状態にする
	if (animController_->IsEnd())
	{
		ChangeState(STATE::STANDBY);
		MV1SetMaterialDifColor(modelId_, 0, COLOR_DIF_DEFAULT);
	}
}

void Player::UpdateAttack(void)
{
	// アニメーションが終わったら、エンド状態にする
	if (animController_->IsEnd())
	{
		ChangeState(STATE::STANDBY);
	}
}

void Player::UpdateDead(void)
{
}