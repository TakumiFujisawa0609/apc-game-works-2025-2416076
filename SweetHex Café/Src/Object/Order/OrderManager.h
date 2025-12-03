#pragma once
#include <vector>
#include "Order.h"

class OrderManager
{
public:

	OrderManager(void);
	~OrderManager(void);

	void Init(void);
	void Load(void);
	void LoadEnd(void);
	void Update(void);
	void Draw(void);
	void Release(void);

	void AddOrder(EnemyBase* enemy);

private:

	std::vector<Order*> orders_;
};