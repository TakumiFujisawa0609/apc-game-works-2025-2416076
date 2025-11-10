#pragma once


class EnemyBase;
class HpManager;

class Order
{
public:
	Order(EnemyBase* enemy);
	~Order(void);
	
	void Init(void);
	void Update(void);
	void Draw(VECTOR pos);
	void Release(void);

	EnemyBase* GetEnemy(void) const;

private:
	EnemyBase* enemy_;
	HpManager* hpManager_;

	VECTOR hpPos_;
};