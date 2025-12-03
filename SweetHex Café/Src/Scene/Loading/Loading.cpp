#include <DxLib.h>
#include "Loading.h"

Loading::Loading(void)
	:
	handle_(-1),
	posX_(0),
	posY_(0),
	isLoading_(false),
	loadTimer_(0)
{
}

Loading::~Loading(void)
{
}

void Loading::Init(void)
{
	loadTimer_ = 0;
	isLoading_ = false;
	posX_ = 0.0f;
	posY_ = 0.0f;
	nowHandle_ = handle_;
}

void Loading::Load(void)
{
	handle_ = LoadGraph("Data/Image/Loading.png");
	handle1_ = LoadGraph("Data/Image/Loading1.png");
	handle2_ = LoadGraph("Data/Image/Loading2.png");
	handle3_ = LoadGraph("Data/Image/Loading3.png");

}

void Loading::Update(void)
{
	loadTimer_++;

	// 読み込み中の物がなくなったもしくは、最低ロード時間経過
	if (GetASyncLoadNum() == 0 && loadTimer_ >= MIN_LOAD_TIME)
	{
		// ロード終了
		Init();
	}
	else
	{
		// ロード画面を動作させるならここに記述
		if (loadTimer_ % MIN_LOAD_TIME >= 60)
		{
			nowHandle_ = handle3_;
		}
		else if (loadTimer_ % MIN_LOAD_TIME >= 40)
		{
			nowHandle_ = handle2_;
		}
		else if (loadTimer_ % MIN_LOAD_TIME >= 20)
		{
			nowHandle_ = handle1_;
		}
		else if (loadTimer_ % MIN_LOAD_TIME >= 0)
		{
			nowHandle_ = handle_;
		}
	}
}

void Loading::Draw(void)
{
	DrawGraphF(
		posX_, posY_,
		nowHandle_,
		true
	);
}

void Loading::Release(void)
{
	DeleteGraph(handle_);
	DeleteGraph(handle1_);
	DeleteGraph(handle2_);
	DeleteGraph(handle3_);
}

void Loading::StartAsyncLoad(void)
{
	isLoading_ = true;
	// 非同期読み込み開始
	SetUseASyncLoadFlag(true);
}

void Loading::EndAsyncLoad(void)
{
	// 非同期読み込み終了
	SetUseASyncLoadFlag(false);
}