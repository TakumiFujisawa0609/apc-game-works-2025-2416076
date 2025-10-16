#pragma once
#include "EnemyBase.h"

class EnemySlime : public EnemyBase
{
public:

	// 衝突判定用半径
	static constexpr float COLLISION_RADIUS = 50.0f;

	// コンストラクタ
	EnemySlime(BlockManager* block);

	// デストラクタ
	~EnemySlime(void);

protected:
	void SetParam(void) override;
};