#include <string>
#include <fstream>
#include <sstream>
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
	baseBlockModelIds_[6] = MV1LoadModel((PATH + "countertop_corner_inner.mv1").c_str());
	baseBlockModelIds_[7] = MV1LoadModel((PATH + "countertop_counter_outer.mv1").c_str());
	baseBlockModelIds_[8] = MV1LoadModel((PATH + "countertop_straight_A_large.mv1").c_str());
	baseBlockModelIds_[9] = MV1LoadModel((PATH + "cash_register.mv1").c_str());
	baseBlockModelIds_[10] = MV1LoadModel((PATH + "coffee_machine.mv1").c_str());
	baseBlockModelIds_[11] = MV1LoadModel((PATH + "stand_mixer.mv1").c_str());
	baseBlockModelIds_[12] = MV1LoadModel((PATH + "display_case_long.mv1").c_str());

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
		int z = 0;

		while (std::getline(ifs, line) && z < NUM_BLOCK_Z)
		{
			std::stringstream ss(line);
			std::string cell;
			int x = 0;

			while (std::getline(ss, cell, ',') && x < NUM_BLOCK_X)
			{
				int chipNo = 0;
				float angle = 0.0f;
				int posY = 0;

				// 最初の":"を検索
				size_t pos1 = cell.find(':');
				if(pos1 != std::string::npos) 
				{
					// 2つ目の":"を検索
					size_t pos2 = cell.find(':', pos1 + 1);
					if (pos2 != std::string::npos)
					{
						chipNo = std::stoi(cell.substr(0, pos1));
						angle = std::stof(cell.substr(pos1 + 1, pos2 - (pos1 - 1)));
						posY = std::stoi(cell.substr(pos2 + 1));
					}
					else
					{
						chipNo = std::stoi(cell.substr(0, pos1));
						angle = std::stof(cell.substr(pos1 + 1));
					}
				} 
				else 
				{
					// ":"がない場合はタイルIDのみとして扱う
					chipNo = std::stoi(cell);
					angle = 0.0f;	// 角度は0に設定
					posY = 0;		// Y位置は0に設定
				}

				// int型をキャストして enum classに変換
				Block::TYPE type = static_cast<Block::TYPE>(chipNo);

				// 先に読み込んでいたモデルのハンドルIDを取得する
				int baseModelId = baseBlockModelIds_[chipNo];

				// ブロックを生成
				Block* block = new Block();
				block->Create(type, baseModelId, x, y, z, angle, posY);

				// 3次元配列にブロッククラスのポインタを格納
				blocks_[y][z][x] = block;

				x++;
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
