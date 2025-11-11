#include <DxLib.h>

#include "Inventory.h"

Inventory::Inventory(void)
    :
    inventoryImage_(-1),
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

void Inventory::Draw(void)
{
    DrawFormatString(DEFAULT_POS_X, DEFAULT_POS_Y, 0x000000, "手持ちアイテム数 × %d", itemCount_);
}

void Inventory::Release(void)
{
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
