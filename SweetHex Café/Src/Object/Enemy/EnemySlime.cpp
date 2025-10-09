#include "EnemySlime.h"

EnemySlime::EnemySlime(void)
{
}

EnemySlime::~EnemySlime(void)
{
}

void EnemySlime::SetParam(void)
{
	scales_ = { 60.0f, 60.0f, 60.0f };
	speed_ = 4.0f;
	reactCnt_ = 0;
	hp_ = 2;
	spawnRange_ = 1.0f;
	collisionRadius_ = COLLISION_RADIUS;
	isAlive_ = true;
}
