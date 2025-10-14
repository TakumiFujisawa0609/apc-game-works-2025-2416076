#pragma once
#include <vector>
#include <map>
#include <DxLib.h>

class Player;
class AnimationController;
class BlockManager;

class EnemyBase
{
public:

	// 標準の拡散光色
	static constexpr COLOR_F COLOR_DIF_DEFAULT = { 0.4f, 0.4f, 0.4f, 1.0f };

	// 点滅時の拡散光色
	static constexpr COLOR_F COLOR_DIF_BLINK = { 1.0f, 0.0f, 0.0f, 1.0f };

	// 点滅間隔
	static constexpr int TERM_BLINK = 8;

	// 被ダメ時間
	static constexpr int CNT_HIT_REACT = 40;

	// 死亡時間
	static constexpr int CNT_DEAD_REACT = 80;

	static constexpr int MAX_HP = 2;

	static constexpr int RUN_SPEED = 4;

	// 視野の広さ
	static constexpr float VIEW_RANGE = 600.0f;

	// 視野角
	static constexpr float VIEW_ANGLE = 30.0f;

	// 90度（ラジアン）
	const float HALF_PI = DX_PI_F / 2.0f;

	// エネミー種別
	enum class TYPE
	{
		SLIME,
		MAX,
	};

	// 行動パターン
	enum class PATTERN
	{
		PATTERN_1,
		PATTERN_2,
		PATTERN_3,
		PATTERN_4,
		PATTERN_5,
		MAX,
	};

	const std::map<PATTERN, std::vector<VECTOR>> ENEMY_MOVE_ROUTES =
	{

		{ PATTERN::PATTERN_1,
		   { { 200, 10, 300 }, { 200, 10, 850 }, { 800, 10, 850 } } 
		},

		{ PATTERN::PATTERN_2,
			{ { 1050, 10, 120 }, { 1650, 10, 120 }, { 1650, 10, 450 } }
		},

		{ PATTERN::PATTERN_3,
			{ { 510, 10, 500 }, {1050, 10, 500} }
		},

		{ PATTERN::PATTERN_4,
			{ { 2200, 10, 500 }, { 2200, 10, 162 }, { 2700,10, 162 } }
		},
		
		{ PATTERN::PATTERN_5,
			{ { 2150, 10, 534 }, { 2150, 10, 1290}, {2770, 10, 1290}, {2770, 10, 370}}
		}
	};

	// 4方向の衝突情報
	struct SurroundingHits
	{
		bool hitForward;
		bool hitBack;
		bool hitRight;
		bool hitLeft;
	};

	// アニメーション種別
	enum class ANIM_TYPE
	{
		ANGER,			// 怒り
		EXCITE,			// わくわく
		SAD,			// 悲しい
		IDLE,			// 待機
		IDLE_BREAK,		// 待機崩れる
		JUMP,			// ジャンプ
		MOVE,			// 移動
		SQUISH_END,		// へこみ終了
		SQUISH_START,	// へこみ開始
		WIGGIE,			// ゆらゆら
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

	void Init(TYPE type, int baseModelId, Player* player, PATTERN pattern);
	void Update(void);
	void Draw(void);
	void Release(void);

	// 状態遷移
	void ChangeState(STATE state);
	VECTOR GetPos(void)const;
	void SetPos(VECTOR pos);
	float GetRadius(void)const;
	VECTOR GetDir(void)const;
	STATE GetState(void)const;
	bool IsAlive(void)const;
	void SetAlive(bool isAlive);
	SurroundingHits CheckCollision(const BlockManager* block);
	PATTERN GetPattern(void)const;
	bool IsNotice(void)const;

	// ダメージを与える
	void Damage(int damage);

	// 衝突判定が有効な状態
	bool IsCollisionState(void)const;
	bool IsCollisionStage(void)const;

protected:
	Player* player_;

	int modelId_;
	VECTOR pos_;
	VECTOR angles_;
	VECTOR scales_;
	VECTOR moveDir_;
	TYPE type_;

	PATTERN currentPattern_;

	std::vector<VECTOR> currentRoute_;

	// 向かっている目的地点のインデックス
	int currentTargetIndex_;

	// 往復するか
	bool isReturn_;

	float speed_;

	int reactCnt_;

	int hp_;

	float spawnRange_;

	STATE state_;

	int cntAttack_;

	// 検知フラグ
	bool isNotice_;

	SurroundingHits hitsResult = { false, false, false, false };

	// 衝突判定用半径
	float collisionRadius_;

	bool isAlive_;

	// コピーコンストラクタ
	EnemyBase(const EnemyBase& instance) = default;

	// アニメーション制御
	AnimationController* animController_;

	void LookPlayer(void);
	void Move(void);
	void MovePattern(void);

	void Search(void);

	// 視野描画
	void DrawViewRange(void);

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