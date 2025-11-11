#pragma once
#include <DxLib.h>

class Counter
{
public:

	Counter(void);
	~Counter(void);

	void Init(void);
	void DrawDebug(void);

	VECTOR GetStartCapsulePos(void)const { return startCapsulePos_; }
	VECTOR GetEndCapsulePos(void)const { return endCapsulePos_; }
	float GetRadius(void)const { return collisionRadius_; }


private:

	static constexpr float COUNTER_RADIUS = 80.0f;
	static constexpr VECTOR START_CAPSULE_POS = { 460.0f, 10.0f, 1500.0f };
	static constexpr VECTOR END_CAPSULE_POS = { 520.0f, 10.0f, 1500.0f };
	static constexpr int DIV_NUM = 6;


	// カプセルの当たり判定座標
	VECTOR startCapsulePos_;
	VECTOR endCapsulePos_;
	float collisionRadius_;

};