#pragma once
#include <DxLib.h>

#include "../../Application.h"

class AnimationController;
class BlockManager;
class WeaponBase;
class WeaponPunch;
class ItemManager;
class HpManager;

class Player
{
public:

	// アニメーション種別
	enum class ANIM_TYPE
	{
		DEATH,
		DEFEAT,
		IDLE,
		JUMP,
		PICKUP,
		PUNCH,
		RECIEVE_HIT,
		ROLL,
		RUN,
		RUN_CARRY,
		SHOOT_ONEHANDED,
		SIT_DOWN,
		STAND_UP,
		SWORDSLACH,
		VICTORY,
		WALK,
		WALK_CARRY,
		MAX,
	};

	// 状態
	enum class STATE
	{
		NONE,
		STANDBY,
		KNOCKBACK,
		ATTACK,
		DEAD,
	};

	// 4方向の衝突情報
	struct SurroundingHits
	{
		bool hitForward;
		bool hitBack;
		bool hitRight;
		bool hitLeft;
		bool hitForwardRight;
		bool hitForwardLeft;
		bool hitBackRight;
		bool hitBackLeft;
	};

	// モデルの大きさ
	static constexpr VECTOR SCALES = { 0.5f, 0.5f, 0.5f };

	// 初期位置
	static constexpr VECTOR DEFAULT_POS = { 300.0f, 10.0f, 300.0f };

	COLOR_F COLOR_EMI_DEFAULT = { 0.0f, 0.0f, 0.3f, 1.0f };

	// 標準の拡散光色
	static constexpr COLOR_F COLOR_DIF_DEFAULT = { 0.0f, 0.0f, 0.0f, 1.0f };

	// 点滅時の拡散光色
	static constexpr COLOR_F COLOR_DIF_BLINK = { 8.0f, 0.2f, 0.2f, 1.0f };

	// ノックバックスピード
	static constexpr float SPEED_KNOCKBACK = 10.0f;

	// 点滅間隔
	static constexpr int TERM_BLINK = 10;

	// 最大HP
	static constexpr int MAX_HP = 3;

	// 移動速度
	static constexpr float MOVE_SPEED = 6.0f;

	// ダッシュ移動速度
	static constexpr float DASH_SPEED = 12.0f;

	// 当たり判定用前方範囲
	static constexpr float COL_RANGE = 20.0f;

	// 衝突判定用半径
	static constexpr float COLLISION_RADIUS = 50.0f;

	// 無敵時間
	static constexpr int INVINCIBLE_TIME = 60;

	// HP描画位置
	static constexpr VECTOR HP_DRAW_POS = { Application::SCREEN_SIZE_X - 80, 30.0f, 0.0f };

public:

	Player(ItemManager* item, BlockManager* block);
	~Player(void);

	void Init(void);
	void Update(void);
	void Draw(BlockManager* block);
	void Release(void);

	VECTOR GetPos(void)const;
	void SetPos(VECTOR pos);

	SurroundingHits CheckCollision(void);
	void CollisionWeapon(BlockManager* block);

	// 状態の変更
	void ChangeState(STATE state);

	// ダメージを与える
	void Damage(int damage);

	WeaponBase* GetUseWeapon(void);

	bool IsInvincible(void);

	bool IsCollisionStage(void)const;
	bool IsCollisionState(void)const;

private:
	// アニメーション
	AnimationController* animController_;

	// 武器
	WeaponBase* useWeapon_;
	WeaponPunch* weaponPunch_;

	ItemManager* item_;

	HpManager* hpManager_;

	BlockManager* block_;

	// モデルのハンドルID
	int modelId_;
	VECTOR pos_;
	VECTOR angles_;
	VECTOR localAngles_;
	VECTOR scales_;

	// 移動方向
	VECTOR moveDir_; 

	float speed_;

	SurroundingHits hitsResult = { false, false, false, false };

	// ノックバック方向
	VECTOR knockBackDir_;
	// ノックバックカウンタ(点滅用)
	int cntKnockBack_;

	// 無敵時間カウンタ
	int invincibleTimeCount_;

	// 衝突判定用半径
	float collisionRadius_;

	int hp_;

	// 現在の状態
	STATE state_;

private:
	void ProcessAttack(void);
	// 行動制御
	void ProcessMove(void);

	void ProcessPickup(void);

	// 状態遷移
	void ChangeStandby(void);
	void ChangeKnockback(void);
	void ChangeAttack(void);
	void ChangeDead(void);

	// 状態別更新
	void UpdateStandby(void);
	void UpdateKnockback(void);
	void UpdateAttack(void);
	void UpdateDead(void);

	void InitTransformPost(void);

	// 遅延回転処理
	void DelayRotate(void);
};