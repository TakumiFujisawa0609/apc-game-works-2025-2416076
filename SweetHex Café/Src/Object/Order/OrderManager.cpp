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
	for (Order* order : orders_)
	{
		order->Init();
	}
}

void OrderManager::Load(void)
{

}

void OrderManager::LoadEnd(void)
{
	Init();
}

void OrderManager::Update(void)
{
	// イテレータを回す
	auto it = orders_.begin();

	while (it != orders_.end())
	{
		Order* order = *it;
		EnemyBase* enemy = order->GetEnemy();

		if (enemy == nullptr || enemy->GetHp() <= 0 || enemy->IsDoor())
		{
			order->Release();
			delete order;

			it = orders_.erase(it);
		}
		else
		{
			order->Update();
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

		orders_[i]->Draw(pos);
	}
}

void OrderManager::Release(void)
{
	for (Order* order : orders_)
	{
		delete order;
	}
	orders_.clear();
}

void OrderManager::AddOrder(EnemyBase* enemy)
{
	Order* newOrder = new Order(enemy);

	newOrder->Load();
	newOrder->LoadEnd();

	orders_.emplace_back(newOrder);
}