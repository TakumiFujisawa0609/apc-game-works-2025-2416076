#include <DxLib.h>

#include "Inventory.h"

Inventory::Inventory(void)
    :
    inventoryImage1_(-1),
    inventoryImage2_(-1),
    itemCount_(0),
    itemImg_(-1)
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
	itemImg_ = LoadGraph((Application::PATH_IMAGE + "item.png").c_str());
}

void Inventory::LoadEnd(void)
{
    Init();
}

void Inventory::Draw(void)
{
    DrawRotaGraph(Application::SCREEN_SIZE_X - DEFAULT_POS_X, DEFAULT_POS_Y, 0.7, 0.0, itemImg_, true);
    DrawFormatString(Application::SCREEN_SIZE_X - 50, 165, 0x000000, "×%d", itemCount_);

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
    DeleteGraph(itemImg_);
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
