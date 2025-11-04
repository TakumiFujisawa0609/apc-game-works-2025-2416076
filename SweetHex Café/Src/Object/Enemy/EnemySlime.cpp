#include "../../Utility/Utility.h"

#include "EnemySlime.h"

EnemySlime::EnemySlime(void)
{
}

EnemySlime::~EnemySlime(void)
{
}

void EnemySlime::SetParam(void)
{
	pos_ = { 132.0f, 10.0f, 168.0f };
	angles_ = { 0.0f, Utility::Deg2RadF(90.0f), 0.0f };
	scales_ = { 60.0f, 60.0f, 60.0f };
	speed_ = 2.0f;
	reactCnt_ = 0;
	hp_ = MAX_HP;
	spawnRange_ = 1.0f;
	collisionRadius_ = COLLISION_RADIUS;
	startCapsulePos_ = { 0.0f,70,0.0f };
	endCapsulePos_ = { 0.0f,70.0f,0.0f };
	isAlive_ = true;

}
