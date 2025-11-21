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

    if (itemCount_ > 0)
    {
        DrawString(TARGET_POS_X, TARGET_POS_Y, "アイテムを提供しよう！", 0x000000);
    }
    else
    {
        DrawString(TARGET_POS_X, TARGET_POS_Y, "敵を殺してアイテムを拾おう！", 0x000000);
    }
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
