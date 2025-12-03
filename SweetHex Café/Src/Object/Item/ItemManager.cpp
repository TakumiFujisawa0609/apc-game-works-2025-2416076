#include "ItemManager.h"

ItemManager::ItemManager(void)
{
}

ItemManager::~ItemManager(void)
{
}

void ItemManager::Init(void)
{
	// リストをクリア
	droppedItems_.clear();
}

void ItemManager::Load(void)
{
}

void ItemManager::LoadEnd(void)
{
    Init();
}

void ItemManager::Update(void)
{
}

void ItemManager::Draw(void)
{
    // リストに登録された全てのアイテムを描画
    for (const DATA& item : droppedItems_)
    {
        DrawSphere3D(item.pos, ITEM_RADIUS, 16, GetColor(0, 255, 0), GetColor(0, 255, 0), false);
    }
}

void ItemManager::Release(void)
{
    // リストをクリアして、保持している要素を全て解放
    droppedItems_.clear();
}

const std::vector<ItemManager::DATA>& ItemManager::GetDroppedItems(void) const
{
    return droppedItems_;
}

void ItemManager::CreateItem(int modelId, const VECTOR& pos)
{
    // 敵の座標をそのままリストに追加
    droppedItems_.emplace_back(DATA{ modelId, pos });
}

void ItemManager::RemoveItem(size_t index)
{
    if (index < droppedItems_.size())
    {
        // 指定されたインデックスの要素をリストから削除
        droppedItems_.erase(droppedItems_.begin() + index);
    }
}