#pragma once
#include <DxLib.h>

class AnimationController;

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

	// モデルの大きさ
	static constexpr VECTOR SCALES = { 0.5f, 0.5f, 0.5f };
	// 初期位置
	static constexpr VECTOR DEFAULT_POS = { 300.0f, 10.0f, 300.0f };

	COLOR_F COLOR_EMI_DEFAULT = { 0.0f, 0.0f, 0.3f, 1.0f };

	// 標準の拡散光色
	static constexpr COLOR_F COLOR_DIF_DEFAULT = { 0.0f, 0.0f, 0.0f, 1.0f };

	// 点滅時の拡散光色
	static constexpr COLOR_F COLOR_DIF_BLINK = { 1.0f, 0.2f, 0.2f, 1.0f };

	// ノックバックスピード
	static constexpr float SPEED_KNOCKBACK = 10.0f;

	// 点滅間隔
	static constexpr int TERM_BLINK = 3;

	// 最大HP
	static constexpr int MAX_HP = 3;

	// 移動速度
	static constexpr float MOVE_SPEED = 3.0f;

	Player(void);
	~Player(void);

	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);

	VECTOR GetPos(void)const;

	// 状態の変更
	void ChangeState(STATE state);

	// 衝突判定
	void CollisionStage(VECTOR pos);

	// ダメージを与える
	void Damage(int damage);

private:
	AnimationController* animController_;

	// モデルのハンドルID
	int modelId_;
	VECTOR pos_;
	VECTOR angles_;
	VECTOR scales_;

	// 移動方向
	VECTOR moveDir_; 

	// ノックバック方向
	VECTOR knockBackDir_;
	// ノックバックカウンタ(点滅用)
	int cntKnockBack_;

	int hp_;

	// 現在の状態
	STATE state_;

private:
	// 行動制御
	void ProcessMove(void);
	void ProcessAttack(void);

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
};