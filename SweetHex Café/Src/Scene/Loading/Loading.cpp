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
}

void Loading::Load(void)
{
	handle_ = LoadGraph("Data/Image/Loading.png");
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
	}
}

void Loading::Draw(void)
{
	DrawGraphF(
		posX_, posY_,
		handle_,
		true
	);
}

void Loading::Release(void)
{
	DeleteGraph(handle_);
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