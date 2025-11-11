#include <DxLib.h>

#include "../../Utility/Utility.h"

#include "../Enemy/EnemyBase.h"
#include "../Common/HP/HpManager.h"

#include "Order.h"

Order::Order(EnemyBase* enemy)
	:
	enemy_(enemy),
	hpManager_(nullptr),
	hpPos_(Utility::VECTOR_ZERO)
{
}

Order::~Order(void)
{
}

void Order::Init(void)
{
	VECTOR hpPos = VGet(0.0f, 0.0f, 0.0f);
	hpManager_ = new HpManager(hpPos, enemy_->GetHp(), enemy_->GetMaxHp(), 0.3f, 30, HpManager::TYPE::UI);
	hpManager_->Init();
}

void Order::Update(void)
{
	if (!enemy_) return;

	// “G‚ÌŒ»Ý‚ÌHP‚ð”½‰f
	hpManager_->SetHp(enemy_->GetHp());
	hpManager_->Update();
}

void Order::Draw(VECTOR pos)
{
	if (!enemy_) return;

	// ’•¶”Ô†
	DrawFormatString(pos.x + 10, pos.y, 0x000000, "#%d ƒ[ƒŠ[~1", enemy_->GetOrderId());

	// HP
	hpManager_->SetPos(VGet((float)(pos.x + 15), (float)(pos.y + 30), 0.0f));
	hpManager_->Draw();

	// ’ñ‹ŸŽžŠÔ
	float timeRate = enemy_->GetServeTime() / enemy_->SERVE_MAX_TIME;
	DrawBox(pos.x + 10, pos.y + 45, pos.x + 10 + (int)(timeRate * 100), pos.y + 55, 0x00ff00, true);
	DrawBox(pos.x + 10, pos.y + 45, pos.x + 110, pos.y + 55, 0xffffff, false);
}

void Order::Release(void)
{
	hpManager_->Release();
	delete hpManager_;
}

EnemyBase* Order::GetEnemy(void)const
{
	return enemy_;
}
