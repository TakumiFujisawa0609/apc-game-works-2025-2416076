#pragma once
#include <vector>
#include <DxLib.h>

class Player;
class AnimationController;

class EnemyBase
{
	// 標準の拡散光色
	static constexpr COLOR_F COLOR_DIF_DEFAULT = { 0.4f, 0.4f, 0.4f, 1.0f };

	// 点滅時の拡散光色
	static constexpr COLOR_F COLOR_DIF_BLINK = { 1.0f, 0.2f, 0.2f, 1.0f };

	// 点滅間隔
	static constexpr int TERM_BLINK = 8;

	// 被ダメ時間
	static constexpr int CNT_HIT_REACT = 40;

	// 死亡時間
	static constexpr int CNT_DEAD_REACT = 80;

	// エネミー種別
	enum class TYPE
	{
		SLIME,
		MAX,
	};

	// アニメーション種別
	enum class ANIM_TYPE
	{
		IDLE,
		MOVE,
		MAX,
	};

	// 状態
	enum class STATE
	{
		NONE,
		STANDBY,
		ATTACK,
		HIT_REACT,
		DEAD_REACK,
		END,
	};

	// 標準の自己発光色
	static constexpr COLOR_F COLOR_EMI_DEFAULT = { 0.6f, 0.6f, 0.6f, 1.0f };

	// コンストラクタ・デストラクタ
	EnemyBase(void);
	~EnemyBase(void);

	void Init(TYPE type, int baseModelId, Player* player);
	void Update(void);
	void Draw(void);
	void Release(void);

	// 状態遷移
	void ChangeState(STATE state);
	VECTOR GetPos(void)const;
	float GetRadius(void)const;
	VECTOR GetDir(void)const;
	STATE GetState(void)const;
	bool IsAlive(void);
	void SetAlive(bool isAlive);

	// ダメージを与える
	void Damage(int damage);

	// 衝突判定が有効な状態
	bool IsCollisionState(void);

protected:
	Player* player_;

	int modelId_;
	VECTOR pos_;
	VECTOR angles_;
	VECTOR scales_;
	VECTOR moveDir_;
	TYPE type_;

	float speed_;

	int reactCnt_;

	int hp_;

	float spawnRange_;

	STATE state_;

	int cntAttack_;

	// 衝突判定用半径
	float collisionRadius_;

	bool isAlive_;

	// コピーコンストラクタ
	EnemyBase(const EnemyBase& instance) = default;

	// アニメーション制御
	AnimationController* animController_;

	void LookPlayer(void);
	void Move(void);
	void SetSpawnPosition(void);

	// パラメータ設定(純粋仮想関数)
	virtual void SetParam(void) = 0;

	// 状態遷移
	virtual void ChangeStandby(void);
	virtual void ChangeAttack(void);
	virtual void ChangeHit(void);
	virtual void ChangeDead(void);
	virtual void ChangeEnd(void);

	// 状態別更新
	virtual void UpdateStandby(void);
	virtual void UpdateAttack(void);
	virtual void UpdateHit(void);
	virtual void UpdateDead(void);
	virtual void UpdateEnd(void);

	// 状態別描画
	virtual void DrawStandby(void);
	virtual void DrawAttack(void);
	virtual void DrawHit(void);
	virtual void DrawDead(void);
	virtual void DrawEnd(void);
};