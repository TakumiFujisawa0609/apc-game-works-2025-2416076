#pragma once
#include <DxLib.h>

class Timer
{
public:
	// 時間
	static constexpr float COUNTDOWN_TIME = 180.0f * 1000.0f;

	// タイマーの位置
	static constexpr int DEFAULT_POS_X = 600;
	static constexpr int DEFAULT_POS_Y = 50;

	// コンストラクタ
	Timer(void);
	// デストラクタ
	~Timer(void);

	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);

	//現在の時間を取得
	int GetSecondLeft(void)const { return secondsLeft_; }

	//タイマーが終了したかどうかを取得
	bool GetIsTimeUp(void)const { return secondsLeft_ <= 0; }

	void CountStart(void) {
		startTime_ = GetNowCount();
		COUNTDOWN_TIME / 1000; // 秒単位で初期化
	}

private:
	//開始時間を取得
	int startTime_;

	//タイマー用変数
	int secondsLeft_;

	// 座標
	VECTOR pos_;

	// 前回の秒数
	int prevSeconds_;
};