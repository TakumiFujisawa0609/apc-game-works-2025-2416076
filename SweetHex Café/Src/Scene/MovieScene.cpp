#include "MovieScene.h"
#include "../Object/MoviePlayer/MoviePlayer.h"
#include "../Manager/SceneManager.h"
#include "../Manager/SoundManager/SoundManager.h"
#include "../Manager/InputManager.h"

MovieScene::MovieScene(void)
{
}

MovieScene::~MovieScene(void)
{
}

void MovieScene::Init(void)
{
    moviePlayer_->Init();
    SoundManager::GetInstance()->StopSound();
}

void MovieScene::Load(void)
{
    moviePlayer_ = new MoviePlayer("Data/Movie/SweetHexCafe.mp4");
    moviePlayer_->Load();
}

void MovieScene::LoadEnd(void)
{
    Init();

    moviePlayer_->LoadEnd();
    moviePlayer_->Play();
}

void MovieScene::Update(void)
{
    moviePlayer_->Update();

    if (CheckHitKeyAll() != 0 ||
        moviePlayer_->IsPlayEnd())
    {
        SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE);
    }
}

void MovieScene::Draw(void)
{
    moviePlayer_->Draw();
}

void MovieScene::Release(void)
{
    moviePlayer_->Release();
    delete moviePlayer_;
    moviePlayer_ = nullptr;
}