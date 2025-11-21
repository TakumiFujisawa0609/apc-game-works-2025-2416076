#include "../../Utility/Utility.h"

#include "Counter.h"

Counter::Counter(void)
	:
	startCapsulePos_(Utility::VECTOR_ZERO),
	endCapsulePos_(Utility::VECTOR_ONE),
	collisionRadius_(0.0f)
{
}

Counter::~Counter(void)
{
}

void Counter::Init(void)
{
	startCapsulePos_ = START_CAPSULE_POS;
	endCapsulePos_ = END_CAPSULE_POS;
	collisionRadius_ = COUNTER_RADIUS;
}

void Counter::DrawDebug(void)
{
}