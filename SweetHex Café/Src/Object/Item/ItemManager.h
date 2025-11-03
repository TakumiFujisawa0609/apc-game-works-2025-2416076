#pragma once
#include <vector>
#include <DxLib.h>

class ItemManager
{
public:
	// アイテムのデータ
	struct DATA
	{
		int modelId;
		VECTOR pos;
		float radius;
	};

	// アイテムの半径
	static constexpr float ITEM_RADIUS = 40.0f;

	static constexpr int ITEM_ID_DEFAULT = 1;

	ItemManager(void);
	~ItemManager(void);

	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);

	const std::vector<VECTOR>& GetDroppedItems(void) const;

	// アイテム生成
	void CreateItem(int itemId_, const VECTOR& pos_);

	// 削除
	void RemoveItem(size_t index_);

private:
	// ドロップされた複数のアイテムの位置情報を管理するリスト
	std::vector<VECTOR> droppedItems_;

};

