#include "ItemManager.h"

ItemManager::ItemManager(void)
{
}

ItemManager::~ItemManager(void)
{
}

void ItemManager::Init(void)
{
	// リストをクリアし、初期化
	droppedItems_.clear();
}

void ItemManager::Update(void)
{

}

void ItemManager::Draw(void)
{
    // リストに登録された全てのアイテムを描画
    for (const VECTOR& pos_ : droppedItems_)
    {
        DrawSphere3D(pos_, ITEM_RADIUS, 16, GetColor(0, 255, 0), GetColor(0, 255, 0), false);
    }
}

void ItemManager::Release(void)
{
    // リストをクリアして、保持している要素を全て解放
    droppedItems_.clear();
}

const std::vector<VECTOR>& ItemManager::GetDroppedItems(void) const
{
    return droppedItems_;
}

void ItemManager::CreateItem(int itemId_, const VECTOR& pos_)
{

    // 敵の座標 (pos_) をそのままリストに追加
    droppedItems_.emplace_back(pos_);
}

void ItemManager::RemoveItem(size_t index_)
{
    if (index_ < droppedItems_.size())
    {
        // 指定されたインデックスの要素をリストから削除
        droppedItems_.erase(droppedItems_.begin() + index_);
    }
}
