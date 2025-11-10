#include "EnemyBase.h"

#include "../../Application.h"
#include "../../Manager/SoundManager/SoundManager.h"
#include "../../Utility/Utility.h"
#include "../../Utility/MatrixUtility.h"

#include "../Common/AnimationController.h"
#include "../Player/Player.h"
#include "../Stage/Stage.h"

EnemyBase::EnemyBase(void)
	:
	animController_(nullptr),
	player_(nullptr),
	stage_(nullptr),
	pos_(Utility::VECTOR_ZERO),
	angles_(Utility::VECTOR_ZERO),
	cntAttack_(0),
	collisionRadius_(0.0f),
	currentPattern_(PATTERN::MAX),
	currentTargetIndex_(0),
	endCapsulePos_(Utility::VECTOR_ZERO),
	hp_(0),
	maxHp_(MAX_HP),
	isAlive_(false),
	isNotice_(false),
	isReturn_(false),
	jumpPow_(0.0f),
	modelId_(-1),
	moveDir_(Utility::VECTOR_ZERO),
	reactCnt_(0),
	scales_(Utility::VECTOR_ZERO),
	spawnRange_(0.0f),
	speed_(0.0f),
	startCapsulePos_(Utility::VECTOR_ZERO),
	state_(STATE::NONE),
	type_(TYPE::MAX),
	isRegister_(false),
	serveTime_(0.0f),
	orderId_(-1),
	isOrderAdded_(false)
{
}

EnemyBase::~EnemyBase(void)
{
}

void EnemyBase::Init(TYPE type, int baseModelId, Player* player, PATTERN pattern, Stage* stage)
{
	type_ = type;

	player_ = player;

	stage_ = stage;

	modelId_ = MV1DuplicateModel(baseModelId);

	// パラメータ
	SetParam();

	// 色の調整(自己発光)
	MV1SetMaterialEmiColor(modelId_, 0, COLOR_EMI_DEFAULT);

	// 大きさ
	MV1SetScale(modelId_, scales_);

	isNotice_ = false;

	animController_ = new AnimationController(modelId_);

	for (int i = 0; i < static_cast<int>(ANIM_TYPE::MAX); i++)
	{
		animController_->AddInFbx(i, 30.0f, i);
	}

	animController_->Play(static_cast<int>(ANIM_TYPE::IDLE));

	// 初期状態
	ChangeState(STATE::STANDBY);

	cntAttack_ = 0;

	currentPattern_ = pattern;

	auto it = ENEMY_MOVE_ROUTES.find(pattern);

	if (it != ENEMY_MOVE_ROUTES.end())
	{
		currentRoute_ = it->second;
	}
	else 
	{
		currentRoute_.clear();
	}

	currentTargetIndex_ = 0;
	isReturn_ = false;

	// 提供時間
	serveTime_ = SERVE_MAX_TIME;

	SoundManager::GetInstance()->Play(SoundManager::SE::ENTRY);
}

void EnemyBase::Update(void)
{
	switch (state_)
	{
	case EnemyBase::STATE::STANDBY:
		UpdateStandby();
		break;
	case EnemyBase::STATE::ATTACK:
		UpdateAttack();
		break;
	case EnemyBase::STATE::HIT_REACT:
		UpdateHit();
		break;
	case EnemyBase::STATE::DEAD_REACT:
		UpdateDead();
		break;
	case EnemyBase::STATE::END:
		UpdateEnd();
		break;
	}

	animController_->Update();
}

void EnemyBase::Draw(void)
{
	switch (state_)
	{
	case EnemyBase::STATE::STANDBY:
		DrawStandby();
		break;
	case EnemyBase::STATE::ATTACK:
		DrawAttack();
		break;
	case EnemyBase::STATE::HIT_REACT:
		DrawHit();
		break;
	case EnemyBase::STATE::DEAD_REACT:
		DrawDead();
		break;
	case EnemyBase::STATE::END:
		DrawEnd();
		break;
	}

#ifdef _DEBUG
	//DrawSphere3D(pos_, collisionRadius_, 10, 0x00ff00, 0x00ff00, false);
#endif // _DEBUG
}

void EnemyBase::Release(void)
{
	MV1DeleteModel(modelId_);

	animController_->Release();
	delete animController_;
}

void EnemyBase::ChangeState(STATE state)
{
	state_ = state;

	switch (state_)
	{
	case EnemyBase::STATE::STANDBY:
		ChangeStandby();
		break;
	case EnemyBase::STATE::ATTACK:
		ChangeAttack();
		break;
	case EnemyBase::STATE::HIT_REACT:
		ChangeHit();
		break;
	case EnemyBase::STATE::DEAD_REACT:
		ChangeDead();
		break;
	case EnemyBase::STATE::END:
		ChangeEnd();
		break;
	}
}

VECTOR EnemyBase::GetPos(void)const
{
	return pos_;
}

void EnemyBase::SetPos(VECTOR pos)
{
	pos_ = pos;

	MV1SetPosition(modelId_, pos_);
}

float EnemyBase::GetRadius(void) const
{
	return collisionRadius_;
}

VECTOR EnemyBase::GetDir(void) const
{
	return moveDir_;
}

EnemyBase::STATE EnemyBase::GetState(void)const
{
	return state_;
}

bool EnemyBase::IsAlive(void)const
{
	return isAlive_;
}

void EnemyBase::SetAlive(bool isAlive)
{
	isAlive_ = isAlive;
}

EnemyBase::PATTERN EnemyBase::GetPattern(void) const
{
	return currentPattern_;
}

bool EnemyBase::IsNotice(void) const
{
	return isNotice_;
}

void EnemyBase::Damage(int damage)
{
	// 攻撃中やノックバック中はダメージを受けない
	if (state_ == STATE::HIT_REACT)
	{
		return;
	}

	hp_ -= damage;

	if (hp_ < 0)
	{
		hp_ = 0;
	}

	if (hp_ <= 0)
	{
		ChangeState(STATE::DEAD_REACT);
	}
	else
	{
		ChangeState(STATE::HIT_REACT);
	}
}

bool EnemyBase::IsCollisionState(void)const
{
	return state_ == STATE::STANDBY
		|| state_ == STATE::ATTACK;
}

void EnemyBase::LookPlayer(void)
{
	// プレイヤー（相手）の座標を取得
	VECTOR playerPos = player_->GetPos();

	// 相手へのベクトルを計算
	VECTOR diff = VSub(playerPos, pos_);
	diff.y = 0.0f;

	// ベクトルの正規化で単位ベクトル（方向）を取得
	moveDir_ = VNorm(diff);

	// 方向から角度（ラジアン）に変換
	angles_.y = atan2(moveDir_.x, moveDir_.z);

	// モデルの方向が正の負の方向を向いているので、補正
	angles_.y += Utility::Deg2RadF(180.0f);

	// 回転はY軸のみ
	angles_.x = angles_.z = 0.0f;

	// モデルに角度を設定
	MV1SetRotationXYZ(modelId_, angles_);
}

void EnemyBase::Move(void)
{
	// 移動量を計算する
	VECTOR movePow = VScale(moveDir_, speed_);
	// 移動量処理
	pos_ = VAdd(pos_, movePow);

	// 移動制限処理
	if (pos_.x <= 0.0f)
	{
		pos_.x = 0.0f;
	}

	if (pos_.z <= 0.0f)
	{
		pos_.z = 0.0f;
	}

	// モデルに座標を設定
	MV1SetPosition(modelId_, pos_);
}

void EnemyBase::Search(void)
{
#pragma region 視野
	VECTOR playerPos = player_->GetPos();
	VECTOR enemyPos = pos_;

	playerPos.y = enemyPos.y = 100.0f;

	VECTOR dirEnemy = VNorm(moveDir_);

	VECTOR diff = VSub(playerPos, pos_);

	VECTOR dirPlayerForEnemy = VNorm(diff);

	// 内積を使ってベクトルの比較
	float dot = VDot(dirEnemy, dirPlayerForEnemy);
	float angle = acosf(dot);

	const float viweRad = Utility::Deg2RadF(VIEW_ANGLE);

	float colX = pos_.x - playerPos.x;
	float colZ = pos_.z - playerPos.z;
	float colPos = 0.0f;
	colPos = colX * colX + colZ * colZ;

	// 視野内にいるか確認
	if (angle <= viweRad && colPos <= VIEW_RANGE * VIEW_RANGE)
	{
		MV1_COLL_RESULT_POLY hit = MV1CollCheck_Line(stage_->GetModelId(), -1, enemyPos, playerPos);

		// 視野内でもプレイヤーと敵の間に壁があるなら
		if (hit.HitFlag)
		{
			// 検知しない
			isNotice_ = false;
		}
		else
		{
			// 検知する
			isNotice_ = true;
		}

	}
	else
	{
		isNotice_ = false;
	}
#pragma endregion
}

void EnemyBase::DrawViewRange(void)
{
#pragma region 視野
	MATRIX mat = MGetIdent();
	mat = MatrixUtility::GetMatrixRotateXYZ(angles_);

	const VECTOR dirForwardBase = VGet(0.0f, 0.0f, -1.0f);

	// 前方方向
	VECTOR forward = VTransform(dirForwardBase, mat);

	// 右
	MATRIX rightMat = MMult(mat, MGetRotY(Utility::Deg2RadF(VIEW_ANGLE)));
	VECTOR right = VTransform(dirForwardBase, rightMat);
	// 左
	MATRIX leftMat = MMult(mat, MGetRotY(Utility::Deg2RadF(-VIEW_ANGLE)));
	VECTOR left = VTransform(dirForwardBase, leftMat);

	VECTOR pos0 = pos_;

	VECTOR pos1 = VAdd(pos0, VScale(forward, VIEW_RANGE));
	VECTOR pos2 = VAdd(pos0, VScale(left, VIEW_RANGE));
	VECTOR pos3 = VAdd(pos0, VScale(right, VIEW_RANGE));

	pos0.y = pos1.y = pos2.y = pos3.y = 10.0f;

	if (isNotice_)
	{
		DrawTriangle3D(pos0, pos2, pos1,
			0xcc44cc, true);

		DrawTriangle3D(pos0, pos1, pos3,
			0xcc44cc, true);
	}
	else
	{
		DrawTriangle3D(pos0, pos2, pos1,
			0x00ff00, true);

		DrawTriangle3D(pos0, pos1, pos3,
			0x00ff00, true);
	}

#pragma endregion
}

void EnemyBase::MovePattern(void)
{
	if (currentRoute_.empty())
	{
		// ルートが定義されていない場合は移動しない
		return;
	}

	// 目標地点を取得
	VECTOR targetPos = currentRoute_[currentTargetIndex_];

	// 目標地点への方向ベクトルを計算し、モデルの向きを設定
	VECTOR diff = VSub(targetPos, pos_);
	diff.y = 0.0f;

	float distance = VSize(diff);

	if (distance < speed_)
	{
		// 到達判定
		SetPos(targetPos); // 正確に目標地点へ移動

		// パターンがレジなら往復しない
		if (currentPattern_ == PATTERN::REGISTER)
		{
			SetRegister(true);
			return;
		}

		if (isReturn_)
		{
			currentTargetIndex_--;

			if (currentTargetIndex_ < 0)
			{
				currentTargetIndex_ = 1;
				isReturn_ = false;
			}
		}
		else
		{
			currentTargetIndex_++;

			if (currentTargetIndex_ >= currentRoute_.size())
			{
				currentTargetIndex_ = static_cast<int>(currentRoute_.size()) - 2;

				if (currentTargetIndex_ < 0)
				{
					currentTargetIndex_ = 0;
					isReturn_ = false;
				}

				isReturn_ = true;
			}
		}
	}
	else
	{
		// 目標地点に向かって移動
		moveDir_ = VNorm(diff);

		// モデルの向きを更新
		angles_.y = atan2(moveDir_.x, moveDir_.z) + Utility::Deg2RadF(180.0f);
		MV1SetRotationXYZ(modelId_, angles_);

		// 移動
		VECTOR movePow = VScale(moveDir_, speed_);
		SetPos(VAdd(pos_, movePow));
	}
}

void EnemyBase::ChangeStandby(void)
{
	MV1SetMaterialEmiColor(modelId_, 0, COLOR_DIF_DEFAULT);
}

void EnemyBase::ChangeAttack(void)
{
	speed_ = RUN_SPEED;
	MV1SetMaterialEmiColor(modelId_, 0, COLOR_DIF_BLINK);
	animController_->Play(static_cast<int>(ANIM_TYPE::ANGER));
}

void EnemyBase::ChangeHit(void)
{
	// ヒットカウンタリセット
	reactCnt_ = 0;
}

void EnemyBase::ChangeDead(void)
{
	animController_->Play(static_cast<int>(ANIM_TYPE::SQUISH_START), false);
	MV1SetMaterialDifColor(modelId_, 0, COLOR_DIF_BLINK);
}

void EnemyBase::ChangeEnd(void)
{
}

void EnemyBase::UpdateStandby(void)
{
	if (hp_ < MAX_HP)
	{
		ChangeState(STATE::ATTACK);
	}

	// 注文を受けたら
	if (isRegister_)
	{
		serveTime_ -= 1.0f;

		if (serveTime_ < 0.0f) serveTime_ = 0.0f;
	}

	MovePattern();

	// 重力処理
	jumpPow_ -= 9.8f;

	// エネミーの座標に移動量を加算
	pos_.y += jumpPow_;

	// エネミーのY座標制限
	if (pos_.y < -9.8f)
	{
		pos_.y = -9.8f;
	}

	MV1SetPosition(modelId_, pos_);

	// 索敵
	Search();
}

void EnemyBase::UpdateAttack(void)
{
	LookPlayer();
	Move();
}

void EnemyBase::UpdateHit(void)
{
	// 指定時間になったら通常状態に戻す
	if (reactCnt_ >= CNT_HIT_REACT)
	{
		ChangeState(STATE::STANDBY);
		return;
	}

	if (reactCnt_ % TERM_BLINK == 0)
	{
		MV1SetMaterialEmiColor(modelId_, 0, COLOR_DIF_DEFAULT);
	}
	else
	{
		MV1SetMaterialEmiColor(modelId_, 0, COLOR_DIF_BLINK);
	}

	reactCnt_++;
}

void EnemyBase::UpdateDead(void)
{
	if (animController_->IsEnd())
	{
		ChangeState(STATE::END);
	}

	if (animController_->IsEnd())
	{
		isAlive_ = false;
	}
}

void EnemyBase::UpdateEnd(void)
{
}


void EnemyBase::DrawStandby(void)
{
	MV1DrawModel(modelId_);

	// 視野描画
	DrawViewRange();
}

void EnemyBase::DrawAttack(void)
{
	MV1DrawModel(modelId_);
}

void EnemyBase::DrawHit(void)
{
	MV1DrawModel(modelId_);
}

void EnemyBase::DrawDead(void)
{
	if (isAlive_)
	{
		MV1DrawModel(modelId_);
	}
}

void EnemyBase::DrawEnd(void)
{
}

void EnemyBase::SetPattern(PATTERN pattern)
{
	currentPattern_ = pattern;
	currentRoute_ = ENEMY_MOVE_ROUTES.at(currentPattern_);
	currentTargetIndex_ = 0;
	isReturn_ = false;
}

void EnemyBase::CollisionStage(const VECTOR& pos)
{
	// 衝突判定に指定座標を押し戻す
	pos_ = pos;
	jumpPow_ = 0.0f;
}