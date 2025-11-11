#include <DxLib.h>

#include "../Enemy/EnemyBase.h"

#include "OrderManager.h"

OrderManager::OrderManager(void)
{
}

OrderManager::~OrderManager(void)
{
}

void OrderManager::Init(void)
{
}

void OrderManager::Update(void)
{
	// イテレータを回す
	auto it = orders_.begin();

	while (it != orders_.end())
	{
		EnemyBase* enemy = it->GetEnemy();

		if (enemy == nullptr || enemy->GetHp() <= 0 || enemy->IsDoor())
		{
			it = orders_.erase(it);
		}
		else
		{
			it->Update();
			++it;
		}
	}
}

void OrderManager::Draw(void)
{
	VECTOR startPos = { 10.0f, 10.0f, 0.0f };

	// 注文1件分の高さ
	int interval = 80;

	for (int i = 0; i < static_cast<int>(orders_.size()); i++)
	{
		VECTOR pos = startPos;

		pos.y += (i * interval);

		orders_[i].Draw(pos);
	}
}

void OrderManager::Release(void)
{
	orders_.clear();
}

void OrderManager::AddOrder(EnemyBase* enemy)
{
	orders_.emplace_back(enemy);
	orders_.back().Init();
}