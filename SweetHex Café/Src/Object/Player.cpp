#include "Player.h"
#include "../Application.h"
#include "../Utility/Utility.h"
#include "../Utility/MatrixUtility.h"
#include "../Manager/InputManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/InputController.h"
#include "../Manager/Camera.h"
#include "Common/AnimationController.h"
#include "Stage/BlockManager.h"

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

	collisionRadius_ = COLLISION_RADIUS;

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

void Player::Draw(BlockManager* block)
{
	MV1DrawModel(modelId_);

#ifdef _DEBUG

	VECTOR playerPos = pos_;
	VECTOR dir = moveDir_;

	const float COLLISION_OFFSET = 50.0f;   // 前方距離
	const float COLLISION_HEIGHT = 10.0f;   // 高さ

	VECTOR startPos = playerPos;
	VECTOR endPos = VAdd(playerPos, VScale(dir, COLLISION_OFFSET));

	startPos.y = endPos.y = COLLISION_HEIGHT;

	DrawSphere3D(startPos, 10, 16, 0xff0000, 0xff0000, true);
	DrawSphere3D(endPos, 10, 16, 0xff0000, 0xff0000, true);


	BlockManager::CollisionResult hit = block->CheckCollisionLine(startPos, endPos);

	if (hit.hit)
	{
		// タグで判定
		if (hit.tag == "FRIDGE")
		{
			// 冷蔵庫に当たった場合の処理
			DrawString(0, 20, "冷蔵庫に当たった", 0x000000);
		}
		else if (hit.tag == "CASH_REGISTER")
		{
			// レジに当たった場合の処理
			DrawString(0, 20, "レジに当たった", 0x000000);
		}
		else if (hit.tag == "DISPLAY_CASE")
		{
			// 提供カウンターに当たった場合の処理
			DrawString(0, 20, "提供カウンターに当たった", 0x000000);
		}
	}

	DrawSphere3D(pos_, collisionRadius_, 10, 0xff0000, 0xff0000, false);

#endif // _DEBUG

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

bool Player::MoveForward(const BlockManager* block)
{
	VECTOR playerPos = pos_;
	VECTOR dir = moveDir_;

	const float COLLISION_OFFSET = 50.0f;   // 前方距離
	const float COLLISION_HEIGHT = 10.0f;   // 高さ

	VECTOR startPos = playerPos;
	VECTOR endPos = VAdd(playerPos, VScale(dir, COLLISION_OFFSET));

	startPos.y = endPos.y = COLLISION_HEIGHT;

	BlockManager::CollisionResult hit = block->CheckCollisionLine(startPos, endPos);

	return !hit.hit; // 当たってなければ進める
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

void Player::ProcessMove(BlockManager* block)
{
	// 攻撃中やノックバック中は移動させない
	if (state_ != STATE::STANDBY)
	{
		return;
	}

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
		//angles_.y = atan2f(moveDir_.x, moveDir_.z);

		if (ins.IsMoveDown() || ins.IsMoveLeft() || ins.IsMoveRight() || ins.IsMoveUp())
		{
			speed_ = Player::MOVE_SPEED;

			if (ins.IsDash())
			{
				speed_ = Player::DASH_SPEED;
			}
		}

		if (MoveForward(block))
		{
			// 移動量を計算する
			VECTOR movePow = VScale(moveDir_, speed_);
			// 移動量処理
			pos_ = VAdd(pos_, movePow);
		}
		else
		{
			speed_ = 0.0f;
		}

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

		// 移動制限処理
		if (pos_.x < 0.0f)
		{
			pos_.x = 0.0f;
		}
		if (pos_.x > BlockManager::WORLD_SIZE)
		{
			pos_.x = BlockManager::WORLD_SIZE;
		}

	}
	else
	{
		animController_->Play(static_cast<int>(ANIM_TYPE::IDLE));
	}

	MV1SetPosition(modelId_, pos_);
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
	// 行列の合成
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
	angles_.y = Utility::LerpAngle(angles_.y, goal, 0.5f);
}