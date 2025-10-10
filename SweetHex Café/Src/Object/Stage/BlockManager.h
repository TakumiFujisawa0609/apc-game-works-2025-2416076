#pragma once
#include <string>
#include <DxLib.h>

class Block;

class BlockManager
{
public:

	struct CollisionResult
	{
		bool hit;
		int modelId;
		VECTOR hitPos;
		std::string tag;
	};

	// ブロック用のモデル種類
	static constexpr int NUM_BLOCK_MODELS = 18;

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
	CollisionResult CheckCollisionLine(VECTOR start, VECTOR end) const;

private:

	// ブロック用のモデルハンドルID
	int baseBlockModelIds_[NUM_BLOCK_MODELS];

	// マップデータ
	Block* blocks_[NUM_BLOCK_Y][NUM_BLOCK_Z][NUM_BLOCK_X];
};