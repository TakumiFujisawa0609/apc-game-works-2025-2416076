#include "Player.h"

#include "../../Utility/Utility.h"
#include "../../Utility/MatrixUtility.h"

#include "../../Manager/InputManager.h"
#include "../../Manager/SceneManager.h"
#include "../../Manager/InputController.h"
#include "../../Manager/SystemManager.h"
#include "../../Manager/SoundManager/SoundManager.h"
#include "../../Manager/Camera.h"

#include "../Common/AnimationController.h"
#include "../Common/HP/HpManager.h"
#include "../Weapon/WeaponPunch.h"
#include "../Item/ItemManager.h"
#include "Inventory.h"

Player::Player(ItemManager* item)
	:
	item_(item),
	animationController_(nullptr),
	hpManager_(nullptr),
	useWeapon_(nullptr),
	inventory_(nullptr),
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
	state_(STATE::NONE),
	isDeath_(false)
{
}

Player::~Player(void)
{
}

void Player::Init(void)
{
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

	animationController_ = new AnimationController(modelId_);

	isDeath_ = false;

	for (int i = 0; i < static_cast<int>(ANIM_TYPE::MAX); i++)
	{
		animationController_->AddInFbx(i, 30.0f, i);
	}

	animationController_->Play(static_cast<int>(ANIM_TYPE::IDLE));

	SceneManager::GetInstance().GetCamera()->SetFollow(this);

	invincibleTimeCount_ = 0;

	ChangeState(STATE::STANDBY);
}

void Player::Load(void)
{
	hp_ = MAX_HP;
	hpManager_ = new HpManager(HP_DRAW_POS, hp_, MAX_HP, 0.5, 30, HpManager::TYPE::UI);
	useWeapon_ = new WeaponPunch();
	inventory_ = new Inventory();
	// モデルのハンドルID
	modelId_ = MV1LoadModel((Application::PATH_MODEL + "Player/Chef_Hat.mv1").c_str());

	hpManager_->Load();
	inventory_->Load();
}

void Player::LoadEnd(void)
{
	Init();

	hpManager_->LoadEnd();
	inventory_->LoadEnd();

	// 武器の初期化
	useWeapon_->Init(WeaponBase::TYPE::PUNCH);
}

void Player::Update(void)
{
	DelayRotate();

	if (useWeapon_ != nullptr)
	{
		useWeapon_->Update();
	}

	if (hpManager_ != nullptr)
	{
		hpManager_->Update();
	}

	animationController_->Update();

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
}

void Player::Draw(void)
{
	MV1DrawModel(modelId_);

	useWeapon_->Draw();

	hpManager_->Draw();

	inventory_->Draw();

	DrawFormatString(DEFAULT_POS_X, DEFAULT_POS_Y, 0x000000, "金額： %d", SystemManager::GetInstance().GetScore());

#ifdef _DEBUG

	//DrawSphere3D(pos_, collisionRadius_, 10, 0xff0000, 0xff0000, false);
	//VECTOR start = VAdd(pos_, startCapsulePos_);
	//VECTOR end = VAdd(pos_, endCapsulePos_);

	//DrawCapsule3D(start, end, collisionRadius_, 10, 0xff0000, 0xff0000, false);

	DrawFormatString(0, 20, 0x000000, " %.2f,%.2f,%.2f", pos_.x, pos_.y, pos_.z);
#endif // _DEBUG

}

void Player::Release(void)
{
	MV1DeleteModel(modelId_);

	animationController_->Release();
	delete animationController_;
	animationController_ = nullptr;

	useWeapon_->Release();
	delete useWeapon_;
	useWeapon_ = nullptr;

	hpManager_->Release();
	delete hpManager_;
	hpManager_ = nullptr;

	inventory_->Release();
	delete inventory_;
	inventory_ = nullptr;
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

	return ret;
}

bool Player::IsCollisionState(void)const
{
	return state_ == STATE::STANDBY
		|| state_ == STATE::ATTACK
		|| state_ == STATE::KNOCKBACK;
}

void Player::CollisionStage(const VECTOR& pos)
{
	// 衝突判定に指定座標を押し戻す
	pos_ = pos;
	jumpPow_ = 0.0f;
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
				animationController_->Play(static_cast<int>(ANIM_TYPE::RUN));
			}
			else
			{
				// アニメーション　通常移動時
				animationController_->Play(static_cast<int>(ANIM_TYPE::WALK));
			}

			if (pos_.z <= 0.0f)
			{
				pos_.z = 0.0f;
			}
		}
		// 移動量を計算する
		VECTOR movePow = VScale(moveDir_, speed_);
		// 移動量処理
		pos_ = VAdd(pos_, movePow);
	}
	else
	{
		animationController_->Play(static_cast<int>(ANIM_TYPE::IDLE));
	}

	MV1SetPosition(modelId_, pos_);
}

void Player::ProcessPickup(void)
{
	auto& ins = InputController::GetInstance();

	const std::vector<ItemManager::DATA>& drop = item_->GetDroppedItems();

		// リストの後ろからチェックする
	for (int i = static_cast<int>(drop.size()) - 1; i >= 0; i--)
	{
		const VECTOR& itemPos = drop[i].pos;

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
				item_->RemoveItem(i);
				inventory_->AddItem(1);
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
	animationController_->Play(static_cast<int>(ANIM_TYPE::IDLE));
}

void Player::ChangeKnockback(void)
{
	animationController_->Play(static_cast<int>(ANIM_TYPE::RECIEVE_HIT), false);
	
	// 無敵時間の設定
	invincibleTimeCount_ = INVINCIBLE_TIME;
}

void Player::ChangeAttack(void)
{
	useWeapon_->Use(pos_, moveDir_);

	animationController_->Play(static_cast<int>(ANIM_TYPE::PUNCH), false);
}

void Player::ChangeDead(void)
{
	animationController_->Play(static_cast<int>(ANIM_TYPE::DEATH), false);
}

void Player::UpdateStandby(void)
{
	// 無敵時間
	if (invincibleTimeCount_ > 0)
	{
		invincibleTimeCount_--;
	}

	ProcessMove();

	// 重力処理
	jumpPow_ -= 9.8f;

	// プレイヤーの座標に移動量を加算
	pos_.y += jumpPow_;

	// プレイヤーのY座標制限
	if (pos_.y < -9.8f)
	{
		pos_.y = -9.8f;
	}

	MV1SetPosition(modelId_, pos_);

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
	if (animationController_->IsEnd())
	{
		ChangeState(STATE::STANDBY);
		MV1SetMaterialDifColor(modelId_, 0, COLOR_DIF_DEFAULT);
	}
}

void Player::UpdateAttack(void)
{
	// アニメーションが終わったら、エンド状態にする
	if (animationController_->IsEnd())
	{
		ChangeState(STATE::STANDBY);
	}
}

void Player::UpdateDead(void)
{
	// アニメーションが終わったら、エンド状態にする
	if (animationController_->IsEnd())
	{
		isDeath_ = true;
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

	// 当たり判定を作成
	startCapsulePos_ = { 0.0f, 100.0f, 0.0f };
	endCapsulePos_ = { 0.0f,70.0f,0.0f };
}

void Player::DelayRotate(void)
{
	// 移動方向から角度に変換する
	float goal = atan2f(moveDir_.x, moveDir_.z);

	// 常に最短経路で補間
	angles_.y = Utility::LerpAngle(angles_.y, goal, 0.5f);
}