#include "Timer.h"

Timer::Timer(void)
    : startTime_(0)
    , secondsLeft_(180)
    , pos_{ DEFAULT_POS_X, DEFAULT_POS_Y, 0}
    , prevSeconds_(0)
{
}

Timer::~Timer(void)
{
}

void Timer::Init(void)
{
    CountStart();
}

void Timer::Update(void)
{
	//タイマーの更新
	int nowTime = GetNowCount();
    int elapsed = (nowTime - startTime_) / 1000;
    secondsLeft_ = (static_cast<int>(COUNTDOWN_TIME) / 1000) - elapsed;

    if (secondsLeft_ < 0)
    {
        // マイナス防止
        secondsLeft_ = 0;
    }
}

void Timer::Draw(void)
{
    // 分
    int minutes = secondsLeft_ / 60;
    // 秒
    int seconds = secondsLeft_ % 60;

    DrawFormatString(
        static_cast<int>(pos_.x),
        static_cast<int>(pos_.y),
        GetColor(0, 0, 0),
        "%02d:%02d",
        minutes, seconds
    );
}

void Timer::Release(void)
{
}

void Timer::CountStart(void)
{
    startTime_ = GetNowCount();
    secondsLeft_ = static_cast<int>(COUNTDOWN_TIME) / 1000;
}
