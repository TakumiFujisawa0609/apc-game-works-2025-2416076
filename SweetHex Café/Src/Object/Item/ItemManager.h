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
		bool isHoldItem;
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

	const std::vector<DATA>& GetDroppedItems(void) const;

	// アイテム生成
	void CreateItem(int modelId, const VECTOR& pos);

	// 削除
	void RemoveItem(size_t index);

private:
	// ドロップされた複数のアイテムの位置情報を管理するリスト
	std::vector<DATA> droppedItems_;

};

