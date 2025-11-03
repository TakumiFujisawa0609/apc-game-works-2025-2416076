#include "Player.h"

#include "../../Utility/Utility.h"
#include "../../Utility/MatrixUtility.h"

#include "../../Manager/InputManager.h"
#include "../../Manager/SceneManager.h"
#include "../../Manager/InputController.h"
#include "../../Manager/SoundManager/SoundManager.h"
#include "../../Manager/Camera.h"

#include "../Common/AnimationController.h"
#include "../Common/HP/HpManager.h"
#include "../Stage/BlockManager.h"
#include "../Weapon/WeaponPunch.h"
#include "../Item/ItemManager.h"

Player::Player(ItemManager* item, BlockManager* block)
	:
	item_(item),
	block_(block),
	animController_(nullptr),
	hpManager_(nullptr),
	useWeapon_(nullptr),
	weaponPunch_(nullptr),
	angles_(Utility::VECTOR_ZERO),
	cntKnockBack_(0),
	collisionRadius_(0.0f),
	hp_(0),
	invincibleTimeCount_(0),
	knockBackDir_(Utility::VECTOR_ZERO),
	localAngles_(Utility::VECTOR_ZERO),
	modelId_(-1),
	moveDir_(Utility::VECTOR_ZERO),
	pos_(Utility::VECTOR_ZERO),
	scales_(Utility::VECTOR_ZERO),
	speed_(0),
	state_(STATE::NONE)
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

	hpManager_ = new HpManager(HP_DRAW_POS, hp_, MAX_HP, 0.5, 30, HpManager::TYPE::UI);
	hpManager_->Init();

	invincibleTimeCount_ = 0;

	// 武器の初期化
	weaponPunch_ = new WeaponPunch();
	weaponPunch_->Init(WeaponBase::TYPE::PUNCH);

	// 初期武器はパンチ
	useWeapon_ = weaponPunch_;

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
	useWeapon_->Update();

	CheckCollision();

	hpManager_->Update();

	animController_->Update();
}

void Player::Draw(BlockManager* block)
{
	MV1DrawModel(modelId_);

	useWeapon_->Draw();

	hpManager_->Draw();

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

	DrawFormatString(0, 40, 0x000000, "%.2f, %.2f,%.2f", pos_.x, pos_.y, pos_.z);


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

	weaponPunch_->Release();
	delete weaponPunch_;

	useWeapon_ = nullptr;

	hpManager_->Release();
	delete hpManager_;
}

VECTOR Player::GetPos(void) const
{
	return pos_;
}

void Player::SetPos(VECTOR pos)
{
	pos_ = pos;

	MV1SetPosition(modelId_, pos_);
}

Player::SurroundingHits Player::CheckCollision(void)
{
	VECTOR playerPos = pos_;
	VECTOR dir = moveDir_;

	const float COLLISION_OFFSET = 50.0f;   // 前方距離
	const float COLLISION_HEIGHT = 10.0f;   // 高さ
	const float HALF_PI = DX_PI_F / 2.0f;   // 90度
	const float QUARTER_PI = HALF_PI / 2.0f;
	VECTOR startPos = playerPos;
	startPos.y = COLLISION_HEIGHT;

	// 前方
	VECTOR forwardEndPos = VAdd(playerPos, VScale(dir, COLLISION_OFFSET));
	forwardEndPos.y = COLLISION_HEIGHT;

	BlockManager::CollisionResult hitForward = block_->CheckCollisionLine(startPos, forwardEndPos);
	hitsResult.hitForward = hitForward.hit;

	if (forwardEndPos.z <= 0)
	{
		hitsResult.hitForward = true;
	}

	// 後方
	VECTOR backwardDir = VScale(dir, -1.0f);
	VECTOR backwardEndPos = VAdd(playerPos, VScale(backwardDir, COLLISION_OFFSET));
	backwardEndPos.y = COLLISION_HEIGHT;

	BlockManager::CollisionResult hitBack = block_->CheckCollisionLine(startPos, backwardEndPos);
	hitsResult.hitBack = hitBack.hit;

	// 右側
	VECTOR rightDir = Utility::RotXZPos(Utility::VECTOR_ZERO, dir, HALF_PI);
	VECTOR rightEndPos = VAdd(playerPos, VScale(rightDir, COLLISION_OFFSET));
	rightEndPos.y = COLLISION_HEIGHT;

	BlockManager::CollisionResult hitRight = block_->CheckCollisionLine(startPos, rightEndPos);
	hitsResult.hitRight = hitRight.hit;

	// 左側
	VECTOR leftDir = Utility::RotXZPos(Utility::VECTOR_ZERO, dir, -HALF_PI);
	VECTOR leftEndPos = VAdd(playerPos, VScale(leftDir, COLLISION_OFFSET));
	leftEndPos.y = COLLISION_HEIGHT;

	BlockManager::CollisionResult hitLeft = block_->CheckCollisionLine(startPos, leftEndPos);
	hitsResult.hitLeft = hitLeft.hit;

	// 右斜め前
	VECTOR forwardRightDir = Utility::RotXZPos(Utility::VECTOR_ZERO, dir, QUARTER_PI);
	VECTOR forwardRightEndPos = VAdd(playerPos, VScale(forwardRightDir, COLLISION_OFFSET));
	forwardRightEndPos.y = COLLISION_HEIGHT;

	BlockManager::CollisionResult hitForwardRight = block_->CheckCollisionLine(startPos, forwardRightEndPos);
	hitsResult.hitForwardRight = hitForwardRight.hit;

	// 左斜め前
	VECTOR forwardLeftDir = Utility::RotXZPos(Utility::VECTOR_ZERO, dir, -QUARTER_PI);
	VECTOR forwardLeftEndPos = VAdd(playerPos, VScale(forwardLeftDir, COLLISION_OFFSET));
	forwardLeftEndPos.y = COLLISION_HEIGHT;

	BlockManager::CollisionResult hitForwardLeft = block_->CheckCollisionLine(startPos, forwardLeftEndPos);
	hitsResult.hitForwardLeft = hitForwardLeft.hit;

	// 右斜め後ろ
	VECTOR backRightDir = Utility::RotXZPos(Utility::VECTOR_ZERO, dir, HALF_PI + QUARTER_PI);
	VECTOR backRightEndPos = VAdd(playerPos, VScale(backRightDir, COLLISION_OFFSET));
	backRightEndPos.y = COLLISION_HEIGHT;

	BlockManager::CollisionResult hitBackRight = block_->CheckCollisionLine(startPos, backRightEndPos);
	hitsResult.hitBackRight = hitBackRight.hit;

	// 左斜め後ろ
	VECTOR backLeftDir = Utility::RotXZPos(Utility::VECTOR_ZERO, dir, -HALF_PI - QUARTER_PI);
	VECTOR backLeftEndPos = VAdd(playerPos, VScale(backLeftDir, COLLISION_OFFSET));
	backLeftEndPos.y = COLLISION_HEIGHT;

	BlockManager::CollisionResult hitBackLeft = block_->CheckCollisionLine(startPos, backLeftEndPos);
	hitsResult.hitBackLeft = hitBackLeft.hit;

	return hitsResult;	// 当たってなければ進める
}

void Player::CollisionWeapon(BlockManager* block)
{
	VECTOR weaponPos = useWeapon_->GetPos();
	float speed = useWeapon_->GetSpeed();
	VECTOR dir = moveDir_;

	VECTOR startPos = weaponPos;
	VECTOR endPos = VAdd(weaponPos, VScale(dir, speed));

	BlockManager::CollisionResult hit = block->CheckCollisionLine(startPos, endPos);

	// 衝突結果
	if (hit.hit) 
	{
		useWeapon_->SetAlive(false);
	}
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
	// 無敵時間中はダメージを受けない
	if (invincibleTimeCount_ > 0)
	{
		return;
	}

	hp_ -= damage;

	hpManager_->SetHp(hp_);

	// 0以下なら、0にする
	if (hp_ <= 0)
	{
		hp_ = 0;
	}

	if (hp_ <= 0)
	{
		ChangeState(STATE::DEAD);
	}
	else
	{
		// ノックバックに状態遷移
		ChangeState(STATE::KNOCKBACK);
	}
}

WeaponBase* Player::GetUseWeapon(void)
{
	return useWeapon_;
}

bool Player::IsInvincible(void)
{
	bool ret = false;

	if (invincibleTimeCount_ > 0)
	{
		ret = true;
	}

	return ret;
}

bool Player::IsCollisionStage(void) const
{
	bool ret = false;

	// どこかが当たってたら
	if (hitsResult.hitForward ||
		hitsResult.hitBack ||
		hitsResult.hitLeft ||
		hitsResult.hitRight ||
		hitsResult.hitForwardRight ||
		hitsResult.hitForwardLeft ||
		hitsResult.hitBackRight ||
		hitsResult.hitBackLeft )
	{
		ret = true;
	}

	return ret;
}

bool Player::IsCollisionState(void)const
{
	return state_ == STATE::STANDBY
		|| state_ == STATE::ATTACK
		|| state_ == STATE::KNOCKBACK;
}

void Player::ProcessMove(void)
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

		bool joypadNum = false;
		if (GetJoypadNum() != 0)
		{
			joypadNum = true;
		}

		if (ins.IsMoveDown() || ins.IsMoveLeft() || ins.IsMoveRight() || ins.IsMoveUp() || joypadNum)
		{
			speed_ = Player::MOVE_SPEED;

			if (ins.IsDash())
			{
				speed_ = Player::DASH_SPEED;

				// アニメーション　ダッシュ時
				animController_->Play(static_cast<int>(ANIM_TYPE::RUN));
			}
			else
			{
				// アニメーション　通常移動時
				animController_->Play(static_cast<int>(ANIM_TYPE::WALK));
			}
		}

		// 前方方向に障害物があるか確認
		if (!hitsResult.hitForward)
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
	}
	else
	{
		animController_->Play(static_cast<int>(ANIM_TYPE::IDLE));
	}

	MV1SetPosition(modelId_, pos_);
}

void Player::ProcessPickup(void)
{
	auto& ins = InputController::GetInstance();

	const std::vector<VECTOR>& drop = item_->GetDroppedItems();

		// リストの後ろからチェックする
	for (int i = static_cast<int>(drop.size()) - 1; i >= 0; i--)
	{
		const VECTOR& itemPos = drop[i];

		// プレイヤーとアイテムの中心間の距離を計算
		VECTOR direction = VSub(pos_, itemPos);
		float distance = sqrtf(direction.x * direction.x
			+ direction.y * direction.y
			+ direction.z * direction.z);

		// プレイヤーとアイテムの衝突判定半径の合計
		float radius = collisionRadius_ + ItemManager::ITEM_RADIUS;

		if (distance <= radius)
		{
			// 半径の合計より小さかったら
			if (ins.IsUse())
			{
				// アイテム取得処理
				// ItemManagerにアイテム削除
				item_->RemoveItem(i);
				SoundManager::GetInstance()->Play(SoundManager::SE::PICKUP);

				break;
			}
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
	
	// 無敵時間の設定
	invincibleTimeCount_ = INVINCIBLE_TIME;
}

void Player::ChangeAttack(void)
{
	useWeapon_->Use(pos_, moveDir_);

	animController_->Play(static_cast<int>(ANIM_TYPE::PUNCH), false);
}

void Player::ChangeDead(void)
{
	animController_->Play(static_cast<int>(ANIM_TYPE::DEATH), false);
}

void Player::UpdateStandby(void)
{
	// 無敵時間
	if (invincibleTimeCount_ > 0)
	{
		invincibleTimeCount_--;
	}

	ProcessMove();

	ProcessPickup();

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
		MV1SetMaterialDifColor(modelId_, 0, COLOR_DIF_BLINK);
	}
	else
	{
		MV1SetMaterialDifColor(modelId_, 0, COLOR_DIF_DEFAULT);
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
	// アニメーションが終わったら、エンド状態にする
	if (animController_->IsEnd())
	{
		SceneManager::GetInstance().ChangeScene(
			SceneManager::SCENE_ID::GAMEOVER);
	}
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