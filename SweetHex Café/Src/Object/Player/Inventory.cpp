#include <DxLib.h>

#include "Inventory.h"

Inventory::Inventory(void)
    :
    inventoryImage1_(-1),
    inventoryImage2_(-1),
    itemCount_(0)
{
}

Inventory::~Inventory(void)
{
}

void Inventory::Init(void)
{
    itemCount_ = 0;
}

void Inventory::Load(void)
{
    inventoryImage1_ = LoadGraph((Application::PATH_IMAGE + "goal1.png").c_str());
    inventoryImage2_ = LoadGraph((Application::PATH_IMAGE + "goal2.png").c_str());
}

void Inventory::LoadEnd(void)
{
    Init();
}

void Inventory::Draw(void)
{
    DrawFormatString(DEFAULT_POS_X, DEFAULT_POS_Y, 0x000000, "手持ちアイテム数 × %d", itemCount_);

    if (itemCount_ > 0)
    {
        DrawRotaGraph(TARGET_POS_X, TARGET_POS_Y, 1.0, 0.0, inventoryImage2_, true);
    }
    else
    {
       DrawRotaGraph(TARGET_POS_X, TARGET_POS_Y, 1.0, 0.0, inventoryImage1_, true);
    }
}

void Inventory::Release(void)
{
    DeleteGraph(inventoryImage1_);
	DeleteGraph(inventoryImage2_);
}

void Inventory::AddItem(int count)
{
    itemCount_ += count;
}

bool Inventory::UseItem(void)
{
    // インベントリのアイテムの数が0より多かったら
    if (itemCount_ > 0)
    {
        // 使える
        itemCount_--;
        return true;
    }
    // 使えない
    return false;
}
