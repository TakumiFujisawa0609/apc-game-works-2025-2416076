#include <string>
#include <fstream>
#include "../../Application.h"
#include "../../Utility/Utility.h"
#include "Block.h"
#include "BlockManager.h"

BlockManager::BlockManager(void)
{
}

BlockManager::~BlockManager(void)
{
}

void BlockManager::Init(void)
{
	// 各種ブロックモデルのロード
	std::string PATH = Application::PATH_MODEL + "Stage/";
	baseBlockModelIds_[0] = MV1LoadModel((PATH + "floor_tiled.mv1").c_str());
	baseBlockModelIds_[1] = MV1LoadModel((PATH + "floor_wood.mv1").c_str());
	baseBlockModelIds_[2] = MV1LoadModel((PATH + "floor_connection.mv1").c_str());
	baseBlockModelIds_[3] = MV1LoadModel((PATH + "wall_panelled_bakery_corner_inner.mv1").c_str());
	baseBlockModelIds_[4] = MV1LoadModel((PATH + "wall_panelled_bakery_straight.mv1").c_str());
	baseBlockModelIds_[5] = MV1LoadModel((PATH + "wall_panelled_bakery_window.mv1").c_str());

	LoadMapCsvData();
}

void BlockManager::Update(void)
{
	for (int y = 0; y < NUM_BLOCK_Y; y++)
	{
		for (int z = 0; z < NUM_BLOCK_Z; z++)
		{
			for (int x = 0; x < NUM_BLOCK_X; x++)
			{
				if (blocks_[y][z][x] != nullptr) 
				{
					blocks_[y][z][x]->Update();

				}
			}
		}
	}
}

void BlockManager::Draw(void)
{

	for (int y = 0; y < NUM_BLOCK_Y; y++)
	{
		for (int z = 0; z < NUM_BLOCK_Z; z++)
		{
			for (int x = 0; x < NUM_BLOCK_X; x++)
			{
				if (blocks_[y][z][x] != nullptr)
				{
					blocks_[y][z][x]->Draw();
				}
			}
		}
	}
}

void BlockManager::Release(void)
{

	for (int y = 0; y < NUM_BLOCK_Y; y++)
	{
		for (int z = 0; z < NUM_BLOCK_Z; z++)
		{
			for (int x = 0; x < NUM_BLOCK_X; x++)
			{
				if (blocks_[y][z][x] != nullptr)
				{
					blocks_[y][z][x]->Release();
					delete blocks_[y][z][x];
					blocks_[y][z][x] = nullptr;
				}
			}
		}
	}

	for (int i = 0; i < NUM_BLOCK_MODELS; i++)
	{
		MV1DeleteModel(baseBlockModelIds_[i]);
	}
}

void BlockManager::LoadMapCsvData(void)
{
	for (int y = 0; y < NUM_BLOCK_Y; y++)
	{
		std::string filePath = Application::PATH_MAP_DATA + std::to_string(y) + ".csv";

		// ファイルの読込
		std::ifstream ifs = std::ifstream(filePath);

		if (!ifs)
		{
			// エラーが発生
			return;
		}

		// ファイルを１行ずつ読み込む
		std::string line; // 1行の文字情報

		std::vector<std::string> strSplit; // 1行を1文字の動的配列に分割

		int chipNo = 0;
		int z = 0;

		while (getline(ifs, line))
		{
			// １行をカンマ区切りで分割
			strSplit = Utility::Split(line, ',');

			for (int x = 0; x < strSplit.size(); x++) {


				// stringからintに変換
				chipNo = stoi(strSplit[x]);

				// int型をキャストして enum classに変換
				Block::TYPE type = static_cast<Block::TYPE>(chipNo);

				// 先に読み込んでいたモデルのハンドルIDを取得する
				int baseModelId = baseBlockModelIds_[chipNo];

				// ブロックを生成
				Block* block = new Block();
				block->Create(type, baseModelId, x, y, z);

				// ２次元配列にブロッククラスのポインタを格納
				blocks_[y][z][x] = block;
			}
			z++;
		}
	}
}

bool BlockManager::IsCollisionLine(VECTOR topPos, VECTOR downPos, MV1_COLL_RESULT_POLY* result)
{
	bool ret = false;
	for (int y = 0; y < NUM_BLOCK_Y; y++)
	{
		for (int z = 0; z < NUM_BLOCK_Z; z++)
		{
			for (int x = 0; x < NUM_BLOCK_X; x++)
			{
				// 2次元配列からBlock情報を取り出す
				Block* block = blocks_[y][z][x];

				if (block != nullptr)
				{

					// 線分とモデルの衝突判定
					MV1_COLL_RESULT_POLY res =
						MV1CollCheck_Line(block->GetModelId(), -1, topPos, downPos);

					if (res.HitFlag)
					{
						// 結果を返す
						*result = res;
						return true;
					}
				}
			}
		}
	}
	return ret;
}
