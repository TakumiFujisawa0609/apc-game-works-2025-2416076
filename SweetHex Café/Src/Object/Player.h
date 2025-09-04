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

	// モデルの大きさ
	static constexpr VECTOR SCALES = { 0.5f, 0.5f, 0.5f };
	// 初期位置
	static constexpr VECTOR DEFAULT_POS = { 300.0f, 10.0f, 300.0f };

	COLOR_F COLOR_EMI_DEFAULT = { 0.0f, 0.0f, 0.3f, 1.0f };

	// 最大HP
	static constexpr int MAX_HP = 5;

	// 移動速度
	static constexpr float MOVE_SPEED = 3.0f;

	Player(void);
	~Player(void);

	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);

	VECTOR GetPos(void)const;

private:
	AnimationController* animController_;

	// モデルのハンドルID
	int modelId_;
	VECTOR pos_;
	VECTOR angles_;
	VECTOR scales_;

	VECTOR moveDir_; // 移動方向

	int hp_;

private:
	// 行動制御
	void ProcessMove(void);
};