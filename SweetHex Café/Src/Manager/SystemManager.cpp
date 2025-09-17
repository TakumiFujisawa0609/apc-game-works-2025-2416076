#include "SystemManager.h"

SystemManager* SystemManager::instance_ = nullptr;

void SystemManager::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = new SystemManager();
	}
}

SystemManager& SystemManager::GetInstance(void)
{
	return *instance_;
}

void SystemManager::Destroy(void)
{
	delete instance_;
	instance_ = nullptr;
}

void SystemManager::ResetGame(void)
{
	score_ = 0; // 各プレイヤーのスコアを0で初期化
}

SystemManager::SystemManager(void)
{
	playerNum_ = 0;
	score_ = 0;
}
