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
		FLOOR_TILED,
		FLOOR_WOOD,
		FLOOR_CONNECTION,
		WALL_INNER,
		WALL_STRANGHT,
		WALL_WINDOW,
	};

public:

	// コンストラクタ
	Block(void);

	// デストラクタ
	~Block(void);

	// ブロックを生成する
	void Create(TYPE type, int baseModelId, int mapX, int mapY, int mapZ);
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
};