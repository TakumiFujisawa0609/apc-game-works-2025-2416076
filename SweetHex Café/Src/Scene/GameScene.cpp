#include <Dxlib.h>
#include "../Application.h"
#include "../Utility/Utility.h"
#include "../Manager/SceneManager.h"
#include "../Manager/InputManager.h"
#include "../Object/Grid.h"
#include "../Object/Player.h"
#include "GameScene.h"

GameScene::GameScene(void)
{
}

GameScene::~GameScene(void)
{
}

void GameScene::Init(void)
{
	grid_ = new Grid();
	grid_->Init();

	player_ = new Player();
	player_->Init();
}

void GameScene::Update(void)
{
	grid_->Update();
	player_->Update();
}

void GameScene::Draw(void)
{
	grid_->Draw();
	player_->Draw();

#ifdef _DEBUG
	DrawString(0, 0, "GameScene", 0xffffff);
#endif // _DEBUG

}

void GameScene::Release(void)
{
	grid_->Release();
	delete grid_;

	player_->Release();
	delete player_;
}
