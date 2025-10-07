#include "Player.h"
#include "../Application.h"
#include "../Utility/Utility.h"
#include "../Utility/MatrixUtility.h"
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

	// 位置設定
	pos_ = DEFAULT_POS;

	speed_ = 0.0f;

	// モデルの角度
	angles_ = Utility::VECTOR_ZERO;
	localAngles_ = { 0.0f, Utility::Deg2RadF(180.0f), 0.0f };

	InitTransformPost();

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
	DelayRotate();

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

void Player::CollisionStage(VECTOR pos)
{
	// 衝突判定に指定座標に押し戻す
	pos_ = pos;
	speed_ = 0.0f;
}

float Player::GetSpeed(void) const
{
	return speed_;
}

VECTOR Player::GetDir(void) const
{
	return moveDir_;
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

VECTOR Player::GetNextPos(float speed) const
{

	VECTOR movePow = VScale(moveDir_, speed);

	return VAdd(pos_, movePow);
}

void Player::ApplyPos(VECTOR newPos)
{
	pos_ = newPos;
	MV1SetPosition(modelId_, pos_);
}

void Player::ProcessMove(void)
{
	InputController& ins = InputController::GetInstance();

	VECTOR moveDir = Utility::VECTOR_ZERO;

	// ゲームパッドの接続数で処理を分ける
	if (GetJoypadNum() == 0)
	{
		// キーが押されたら、その方向に移動する
		if (ins.IsMoveUp()) { moveDir = Utility::DIR_F; }
		if (ins.IsMoveDown()) { moveDir = Utility::DIR_B; }
		if (ins.IsMoveLeft()) { moveDir = Utility::DIR_L; }
		if (ins.IsMoveRight()) { moveDir = Utility::DIR_R; }
	}
	else
	{
		// 接続されているゲームパッド1の情報を取得
		InputManager::JOYPAD_IN_STATE padState =
			InputManager::GetInstance().GetJPadInputState(InputManager::JOYPAD_NO::PAD1);

		// アナログキーの入力値から方向を取得
		moveDir = InputManager::GetInstance().GetDirectionXZAKey(padState.AKeyLX, padState.AKeyLY);
	}

	if (!Utility::EqualsVZero(moveDir))
	{
		MATRIX mat = MGetIdent();
		mat = MMult(mat, MGetRotX(angles_.x));
		//mat = MMult(mat, MGetRotY(angles_.y));
		mat = MMult(mat, MGetRotZ(angles_.z));

		moveDir_ = VTransform(moveDir, mat);

		// 移動方向に合わせてプレイヤーの向きを回転
		angles_.y = atan2f(moveDir_.x, moveDir_.z);

		if (ins.IsMoveDown() || ins.IsMoveLeft() || ins.IsMoveRight() || ins.IsMoveUp())
		{
			speed_ = Player::MOVE_SPEED;

			if (ins.IsDash())
			{
				speed_ = Player::DASH_SPEED;
			}
		}

		// 移動量を計算する
		VECTOR movePow = VScale(moveDir_, speed_);

		// 移動量処理
		pos_ = VAdd(pos_, movePow);

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
	else
	{
		speed_ = 0.0f;
		animController_->Play(static_cast<int>(ANIM_TYPE::IDLE));
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

	// 行列の合成(子, 親と指定すると親⇒子の順に適用される)
	MATRIX mat = MatrixUtility::Multiplication(localAngles_, angles_);

	// 回転行列をモデルに反映
	MV1SetRotationMatrix(modelId_, mat);

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

void Player::InitTransformPost(void)
{
	// 大きさをモデルに反映
	MV1SetScale(modelId_, scales_);

	// 角度から方向に変換する
	moveDir_ = { sinf(angles_.y), 0.0f, cosf(angles_.y) };

	// 行列の合成(子, 親と指定すると親⇒子の順に適用される)
	MATRIX mat = MatrixUtility::Multiplication(localAngles_, angles_);

	// 回転行列をモデルに反映
	MV1SetRotationMatrix(modelId_, mat);

	// 座標をモデルに反映
	MV1SetPosition(modelId_, pos_);
}

void Player::DelayRotate(void)
{
	// 移動方向から角度に変換する
	float goal = atan2f(moveDir_.x, moveDir_.z);

	// 常に最短経路で補間
	angles_.y = Utility::LerpAngle(angles_.y, goal, 0.05f);
}