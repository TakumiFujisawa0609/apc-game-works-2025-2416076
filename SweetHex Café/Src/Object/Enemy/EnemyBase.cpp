#include "../../Application.h"
#include "../../Utility/Utility.h"

#include "../Stage/BlockManager.h"
#include "../Common/AnimationController.h"
#include "../Player/Player.h"

#include "EnemyBase.h"

EnemyBase::EnemyBase(void)
{
}

EnemyBase::~EnemyBase(void)
{
}

void EnemyBase::Init(TYPE type, int baseModelId, Player* player, PATTERN pattern)
{
	type_ = type;

	player_ = player;

	modelId_ = MV1DuplicateModel(baseModelId);

	// パラメータ
	SetParam();

	// 色の調整(自己発光)
	MV1SetMaterialEmiColor(modelId_, 0, COLOR_EMI_DEFAULT);

	// 大きさ
	MV1SetScale(modelId_, scales_);

	SetSpawnPosition();

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
	case EnemyBase::STATE::DEAD_REACK:
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
	case EnemyBase::STATE::DEAD_REACK:
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
	case EnemyBase::STATE::DEAD_REACK:
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

EnemyBase::SurroundingHits EnemyBase::CheckCollision(const BlockManager* block)
{
	VECTOR enemyPos = pos_;
	VECTOR dir = moveDir_;

	const float COLLISION_OFFSET = 70.0f;   // 前方距離
	const float COLLISION_HEIGHT = 10.0f;   // 高さ
	const float HALF_PI = DX_PI_F / 2.0f;   // 90度（ラジアン）

	VECTOR startPos = enemyPos;
	startPos.y = COLLISION_HEIGHT;

	// 前方
	VECTOR forwardEndPos = VAdd(enemyPos, VScale(dir, COLLISION_OFFSET));
	forwardEndPos.y = COLLISION_HEIGHT;

	BlockManager::CollisionResult hitForward = block->CheckCollisionLine(startPos, forwardEndPos);
	hitsResult.hitForward = hitForward.hit;

	// 後方
	VECTOR backwardDir = VScale(dir, -1.0f);
	VECTOR backwardEndPos = VAdd(enemyPos, VScale(backwardDir, COLLISION_OFFSET));
	backwardEndPos.y = COLLISION_HEIGHT;

	BlockManager::CollisionResult hitBack = block->CheckCollisionLine(startPos, backwardEndPos);
	hitsResult.hitBack = hitBack.hit;

	// 右側
	VECTOR rightDir = Utility::RotXZPos(Utility::VECTOR_ZERO, dir, HALF_PI);
	VECTOR rightEndPos = VAdd(enemyPos, VScale(rightDir, COLLISION_OFFSET));
	rightEndPos.y = COLLISION_HEIGHT;

	BlockManager::CollisionResult hitRight = block->CheckCollisionLine(startPos, rightEndPos);
	hitsResult.hitRight = hitRight.hit;

	// 左側
	VECTOR leftDir = Utility::RotXZPos(Utility::VECTOR_ZERO, dir, -HALF_PI);
	VECTOR leftEndPos = VAdd(enemyPos, VScale(leftDir, COLLISION_OFFSET));
	leftEndPos.y = COLLISION_HEIGHT;

	BlockManager::CollisionResult hitLeft = block->CheckCollisionLine(startPos, leftEndPos);
	hitsResult.hitLeft = hitLeft.hit;

	return hitsResult;	// 当たってなければ進める
}

EnemyBase::PATTERN EnemyBase::GetPattern(void) const
{
	return currentPattern_;
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
		ChangeState(STATE::DEAD_REACK);
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

bool EnemyBase::IsCollisionStage(void) const
{
	bool ret = false;

	// どこかが当たってたら
	if (hitsResult.hitForward ||
		hitsResult.hitBack ||
		hitsResult.hitLeft ||
		hitsResult.hitRight)
	{
		ret = true;
	}

	return ret;
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
	// 障害物があるか確認
	if (!hitsResult.hitForward)
	{
		// 移動量を計算する
		VECTOR movePow = VScale(moveDir_, speed_);
		// 移動量処理
		pos_ = VAdd(pos_, movePow);
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

	if (pos_.z < 0.0f)
	{
		pos_.z = 0.0f;
	}
	if (pos_.z > BlockManager::WORLD_SIZE)
	{
		pos_.z = BlockManager::WORLD_SIZE;
	}

	// モデルに座標を設定
	MV1SetPosition(modelId_, pos_);
}

void EnemyBase::SetSpawnPosition(void)
{
}

void EnemyBase::MovePattern(void)
{
	if (currentRoute_.empty())
	{
		// ルートが定義されていないか、空のパターン (例: PATTERN_5, Wait) の場合は移動しない
		return;
	}

	if (currentRoute_.size() <= 1)
	{
		// 1点しか目標がない場合（例: その場待機）は移動しない
		return;
	}

	// 1. 目標地点を取得
	const VECTOR& targetPos = currentRoute_[currentTargetIndex_];

	// 2. 目標地点への方向ベクトルを計算し、モデルの向きを設定
	VECTOR diff = VSub(targetPos, pos_);
	diff.y = 0.0f;

	float distance = VSize(diff);

	if (distance < speed_)
	{
		// 3. 到達判定（目標地点に非常に近づいたら）
		SetPos(targetPos); // 正確に目標地点へ移動

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
				currentTargetIndex_ = currentRoute_.size() - 2;

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
		moveDir_ = VNorm(diff); // 方向を更新

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
	MV1SetMaterialDifColor(modelId_, 0, COLOR_DIF_DEFAULT);
}

void EnemyBase::ChangeAttack(void)
{
	speed_ = RUN_SPEED;
}

void EnemyBase::ChangeHit(void)
{
	// ヒットカウンタリセット
	reactCnt_ = 0;
}

void EnemyBase::ChangeDead(void)
{
	reactCnt_ = 0;
	animController_->Play(static_cast<int>(ANIM_TYPE::SQUISH_START), false);
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
	MovePattern();
}

void EnemyBase::UpdateAttack(void)
{

	LookPlayer();
	Move();

	//if (animController_->IsEnd())
	//{
	//	ChangeState(STATE::STANDBY);
	//}
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
		MV1SetMaterialDifColor(modelId_, 0, COLOR_DIF_DEFAULT);
	}
	else
	{
		MV1SetMaterialDifColor(modelId_, 0, COLOR_DIF_BLINK);
	}

	reactCnt_++;
}

void EnemyBase::UpdateDead(void)
{
	if (reactCnt_ >= CNT_DEAD_REACT && animController_->IsEnd())
	{
		ChangeState(STATE::END);
	}

	if (animController_->IsEnd())
	{
		isAlive_ = false;
	}

	if (reactCnt_ % TERM_BLINK == 0)
	{
		MV1SetMaterialDifColor(modelId_, 0, COLOR_DIF_DEFAULT);
	}
	else
	{
		MV1SetMaterialDifColor(modelId_, 0, COLOR_DIF_BLINK);
	}
}

void EnemyBase::UpdateEnd(void)
{
}

void EnemyBase::DrawStandby(void)
{
	MV1DrawModel(modelId_);
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