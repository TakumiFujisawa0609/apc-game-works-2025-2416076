#include <DxLib.h>
#include "FpsControl.h"
#include "../Application.h"

//　コンストラクタ
FpsControl::FpsControl()
	: currentTime_(0)
	, prevFrameTime_(0)
	, frameCnt_(0)
	, updateFrameRateTime_(0)
	, frameRate_(0.0f)
{
}

//デストラクタ
FpsControl::~FpsControl()
{
}

void FpsControl::Init(void)
{
	currentTime_ = 0;
	prevFrameTime_ = 0;
	frameCnt_ = 0;
	updateFrameRateTime_ = 0;
	frameRate_ = 0.0f;
}

bool FpsControl::UpdateFrameRate(void)
{
	Sleep(1);		//システムに処理を返す

	//現在の自己kを取得
	currentTime_ = GetNowCount();

	//現在の時刻が、前回のフレーム実行時より1/60秒経過していたら実行
	if (currentTime_ - prevFrameTime_ >= FRAME_RATE)
	{
		//フレーム実行時の時間を更新
		prevFrameTime_ = currentTime_;

		//フレーム数をカウント
		frameCnt_++;

		//1/60秒経過
		return true;
	}

	return false;
}

void FpsControl::CalcFrameRate(void)
{
	//前回のフレームレート更新から経過時間を求める
	int difTime = currentTime_ - updateFrameRateTime_;

	//前回のフレームレート更新から１秒以上経過していたら更新
	if (difTime > 1000)
	{
		//フレーム回数をミリ秒に合わせる
		//少数まで出したのでfloatにキャスト
		float castFrameCnt = (float)(frameCnt_ * 1000);

		//フレームを求める
		//理想通りなら　60000　/　1000　で60となる
		frameRate_ = castFrameCnt / difTime;

		//フレームカウントをクリア
		frameCnt_ = 0;

		//フレームレート更新時間を更新
		updateFrameRateTime_ = currentTime_;
	}
}

void FpsControl::DrawFrameRate(void)
{
	if (frameRate_ < 56.0f)
	{
		//フレームレートを表示
		DrawFormatString(
			Application::SCREEN_SIZE_X - 16 * 6,		//ｘ座標
			Application::SCREEN_SIZE_Y - 16,			//ｙ座標
			0xFF0000,									//文字の色
			"FPS[%.2f}",
			frameRate_
		);
	}
	else
	{
		//フレームレートを表示
		DrawFormatString(
			Application::SCREEN_SIZE_X - 16 * 6,		//ｘ座標
			Application::SCREEN_SIZE_Y - 16,			//ｙ座標
			0x000000,									//文字の色
			"FPS[%.2f}",
			frameRate_
		);
	}

}