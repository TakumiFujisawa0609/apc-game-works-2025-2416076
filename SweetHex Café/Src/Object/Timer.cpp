#include "Timer.h"

Timer::Timer(void)
{
}

Timer::~Timer(void)
{
}

void Timer::Init(void)
{
	// 座標を初期化
	pos_ = { DEFAULT_POS_X, DEFAULT_POS_Y };

	// 残り時間を初期化
	secondsLeft_ = 60;
	prevSeconds_ = -1;
}

void Timer::Update(void)
{
	//タイマーの更新
	int elapsedTime = GetNowCount() - startTime_;
	int remainingTime = COUNTDOWN_TIME - elapsedTime;
	secondsLeft_ = remainingTime > 0 ? remainingTime / 1000 : 0;
}

void Timer::Draw(void)
{
    int sec = secondsLeft_;

    int digit1 = sec / 10; // 10の位
    int digit2 = sec % 10; // 1の位

    //if (sec <= 60)
    //{
    //    // 残り時間の数字を表示（画像）
    //    DrawRotaGraph(pos_.x - NUMBER_SIZE_X * 2, pos_.y, SIZE_RATE, 0.0f, image_[digit1], true);   // 10の位
    //    DrawRotaGraph(pos_.x + NUMBER_SIZE_X * 2, pos_.y, SIZE_RATE, 0.0f, image_[digit2], true);   // 1の位
    //}

    // 残り10秒以下で秒数が変わった瞬間にSE再生
    //if (secondsLeft_ <= 10 && secondsLeft_ != prevSeconds_)
    //{
    //    sound_.Play(seCountDownHandle_, true); // trueは多分ループの意味なので、必要ならfalseに
    //}

    prevSeconds_ = secondsLeft_;
}

void Timer::Release(void)
{
}