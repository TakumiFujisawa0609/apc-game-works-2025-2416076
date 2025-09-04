#pragma once
#include <DxLib.h>
class Block;

class BlockManager
{
public:
	// ブロック用のモデル種類
	static constexpr int NUM_BLOCK_MODELS = 13;

	// ブロックの数(マップのサイズ)
	static constexpr int NUM_BLOCK_X = 20;
	static constexpr int NUM_BLOCK_Z = 20;
	static constexpr int NUM_BLOCK_Y = 3;

	// 1ブロックあたりのサイズ
	static constexpr float SIZE_BLOCK = 100.0f;

	// マップ全体のサイズ
	static constexpr float WORLD_SIZE =
		SIZE_BLOCK * static_cast<float>(NUM_BLOCK_X);

public:

	// コンストラクタ
	BlockManager(void);

	// デストラクタ
	~BlockManager(void);

	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);

	void LoadMapCsvData(void);

	// 線分とブロックの衝突判定
	bool IsCollisionLine(
		VECTOR topPos, VECTOR downPos, MV1_COLL_RESULT_POLY* result);

private:

	// ブロック用のモデルハンドルID
	int baseBlockModelIds_[NUM_BLOCK_MODELS];

	// マップデータ
	Block* blocks_[NUM_BLOCK_Y][NUM_BLOCK_Z][NUM_BLOCK_X];
};