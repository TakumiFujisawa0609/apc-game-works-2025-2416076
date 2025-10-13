#pragma once
#include <string>
#include <DxLib.h>

class Block
{
public:
	// ブロックモデルの自己発光色
	static constexpr COLOR_F COLOR_EMI = { 0.5f, 0.5f, 0.5f, 0.5f };

	// ブロックモデルの大きさ
	static constexpr VECTOR SCALES = { 0.5f, 0.5f, 0.5f };

	// ブロック種別
	enum class TYPE
	{
		NONE = -1,
		FLOOR_WOOD,			// 床0
		WALL_INNER,			// 壁の角側1
		WALL_STRANGHT,		// 壁(タイル)2
		WALL_DOOR,			// ドア3
		COUNTER_LARGE,		// カウンター長4
		CASH_REGISTER,		// レジ5
		COFFEE_MACHINE,		// コーヒーマシン6
		STAND_MAXER,		// スタンドミキサー7
		DISPLAY_CASE,		// ディスプレイ8
		STOVE,				// コンロ9
		FRIDGE,				// 冷蔵庫10
		SINK,				// シンク11
		CUTTINGBOARD,		// まな板12
	};

public:

	// コンストラクタ
	Block(void);

	// デストラクタ
	~Block(void);

	// ブロックを生成する
	void Create(TYPE type, int baseModelId, int mapX, int mapY, int mapZ, float angle, int posY);
	void Update(void);
	void Draw(void);
	void Release(void);

	int GetModelId(void) { return modelId_; }
	const std::string& GetTag(void) { return tag_; }

private:

	// ブロック種別
	TYPE type_;

	// モデルのハンドルID
	int modelId_;

	std::string tag_;

	// 座標
	VECTOR pos_;

	// 大きさ
	VECTOR scales_;

	// 角度
	float angle_;
};