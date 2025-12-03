#pragma once
#include "../../Application.h"

class Inventory
{
public:

	Inventory(void);
	~Inventory(void);

	void Init(void);
	void Load(void);
	void LoadEnd(void);
	void Draw(void);
	void Release(void);

	// アイテムを追加
	void AddItem(int count);

	// アイテムを使う
	bool UseItem(void);

private:

	// 手持ちアイテム表示位置
	static constexpr int DEFAULT_POS_X = Application::SCREEN_SIZE_X - 200;
	static constexpr int DEFAULT_POS_Y = 70;

	 // アナウンスの表示位置
	static constexpr int TARGET_POS_X = Application::SCREEN_SIZE_X / 2 - 100;
	static constexpr int TARGET_POS_Y = 10;

	int inventoryImage_;
	int itemCount_;
};

