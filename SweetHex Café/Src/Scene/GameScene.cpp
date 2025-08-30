#include <Dxlib.h>
#include "../Application.h"
#include "../Utility/Utility.h"
#include "../Manager/SceneManager.h"
#include "../Manager/InputManager.h"
#include "../Object/Grid.h"
#include "../Object/Stage/BlockManager.h"
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

	blockManager_ = new BlockManager();
	blockManager_->Init();
}

void GameScene::Update(void)
{
	blockManager_->Update();
	grid_->Update();
}

void GameScene::Draw(void)
{
	grid_->Draw();
	blockManager_->Draw();

#ifdef _DEBUG
	DrawString(0, 0, "GameScene", 0xffffff);
#endif // _DEBUG

}

void GameScene::Release(void)
{
	grid_->Release();
	delete grid_;

	blockManager_->Release();
	delete blockManager_;
}
