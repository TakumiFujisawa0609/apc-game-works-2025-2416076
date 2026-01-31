#include "MoviePlayer.h"
#include "../../Application.h"

MoviePlayer::MoviePlayer(const char* filePath)
	: movieHandle_(-1), filePath_(filePath), isLoadSuccess_(false), state_(STATE::STOP)
{
}

MoviePlayer::~MoviePlayer(void)
{
}

void MoviePlayer::Init(void)
{
	if (movieHandle_ != -1)
	{
		isLoadSuccess_ = true;
		ChangeState(STATE::STOP);
	}
	else
	{
		isLoadSuccess_ = false;
		ChangeState(STATE::ERROR_END);
	}

	ChangeMovieVolumeToGraph(128, movieHandle_);
}

void MoviePlayer::Load(void)
{
	movieHandle_ = LoadGraph(filePath_);
}

void MoviePlayer::LoadEnd(void)
{

}

void MoviePlayer::Update(void)
{
	if (state_ == STATE::STOP || state_ == STATE::END)
	{
		return;
	}

	int dxState = GetMovieStateToGraph(movieHandle_);

	switch (dxState)
	{
	case 0: // í‚é~íÜ
		ChangeState(STATE::END);
		break;
	case 1: // çƒê∂íÜ
		ChangeState(STATE::PLAYING);
		break;
	case -1: // ÉGÉâÅ[
		ChangeState(STATE::ERROR_END);
		break;
	}

}

void MoviePlayer::Draw(void)
{
	if(state_ == STATE::PLAYING)
	{
		DrawExtendGraph(0, 0,
			Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y,
			movieHandle_, true);
	}
}

void MoviePlayer::Release(void)
{
	if (movieHandle_ != -1)
	{
		DeleteGraph(movieHandle_);
		movieHandle_ = -1;
	}
	ChangeState(STATE::STOP);
}

void MoviePlayer::Play(void)
{
	if (!isLoadSuccess_ || state_ == STATE::PLAYING)
	{
		return;
	}

	PlayMovieToGraph(movieHandle_);
	ChangeState(STATE::PLAYING);
}

bool MoviePlayer::IsPlayEnd(void) const
{
	return state_ == STATE::END || state_ == STATE::ERROR_END;
}

bool MoviePlayer::IsLoadSuccess(void) const
{
	return isLoadSuccess_;
}

void MoviePlayer::ChangeState(STATE state)
{
	state_ = state;
}