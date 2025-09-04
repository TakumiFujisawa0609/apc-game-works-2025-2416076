#pragma once
#include <DxLib.h>

class Block
{
public:
	// ブロックモデルの自己発光色
	static constexpr COLOR_F COLOR_EMI = { 0.5f, 0.5f, 0.5f, 1.0f };

	// ブロックモデルの大きさ
	static constexpr VECTOR SCALES = { 0.5f, 0.5f, 0.5f };

	// ブロック種別
	enum class TYPE
	{
		NONE = -1,
		FLOOR_TILED = 0,		// タイルの床
		FLOOR_WOOD = 1,			// 気の床
		FLOOR_CONNECTION = 2,	// ミックスされた床
		WALL_INNER = 3,			// 壁の角側
		WALL_STRANGHT = 4,		// 壁
		WALL_WINDOW = 5,		// 窓付き壁
		COUNTER_INNER = 6,		// カウンター角
		COUNTER_OUTER = 7,		// カウンター短
		COUNTER_LARGE = 8,		// カウンター長
		CASH_REGISTER = 9,		// レジ
		COFFEE_MACHINE = 10,	// コーヒーマシン
		STAND_MAXER = 11,		// スタンドマキサー
		DISPLAY_CASE = 12,		// ディスプレイ
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

	// モデルのハンドルIDを取得する
	int GetModelId(void);

private:

	// ブロック種別
	TYPE type_;

	// モデルのハンドルID
	int modelId_;

	// 座標
	VECTOR pos_;

	// 大きさ
	VECTOR scales_;

	// 角度
	float angle_;
};