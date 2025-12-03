#include <string>

#include "../../../Application.h"

#include "HpManager.h"

HpManager::HpManager(VECTOR pos, int hp, int maxHp,float size, int width, TYPE type)
{
	pos_ = pos;
	hp_ = hp;
	maxHp_ = maxHp;
	size_ = size;
	type_ = type;
	iconWidth_ = width;
}

HpManager::~HpManager(void)
{
}

void HpManager::Init(void)
{
	InitHp();
}

void HpManager::Load(void)
{
	std::string path;

	// ２Ｄ画像のロード
	path = Application::PATH_IMAGE + "Heart.png";
	imgHeart_ = LoadGraph(path.c_str());

	path = Application::PATH_IMAGE + "HeartLost.png";
	imgHeartLost_ = LoadGraph(path.c_str());
}

void HpManager::LoadEnd(void)
{
	Init();
}

void HpManager::Update(void)
{
	for (int i = 0; i < maxHp_; i++)
	{
		// プレイヤーのＨＰ残量に応じて、ハート画像を切り替える
		if (i > hp_ - 1)
		{
			hps_[i].scale -= HP_SCALE_SPEED;

			if (hps_[i].scale <= 0.0f)
			{
				hps_[i].scale = 0.0f;
			}
		}
	}
}

void HpManager::Draw(void)
{

	VECTOR drawPos = pos_;

	if (type_ == TYPE::WORLD)
	{
		VECTOR screen;

		// 3D空間の座標をスクリーン座標に変換
		screen = ConvWorldPosToScreenPos(pos_);

		drawPos = screen;

		// HPが最大じゃなかったら表示する
		if (hp_ != maxHp_)
		{
			for (int i = 0; i < maxHp_; i++)
			{
				float posX = drawPos.x + i * iconWidth_;
				float posY = drawPos.y;

				DrawRotaGraph(static_cast<int>(posX), static_cast<int>(posY), static_cast<double>(size_), 0.0, imgHeartLost_, true);

				if (i > hp_ - 1)
				{
					DrawRotaGraph(static_cast<int>(posX), static_cast<int>(posY), static_cast<double>(hps_[i].scale), 0.0, imgHeart_, true);
				}
				else
				{
					DrawRotaGraph(static_cast<int>(posX), static_cast<int>(posY), static_cast<double>(size_), 0.0, imgHeart_, true);
				}
			}
		}
	}
	else if (type_ == TYPE::UI)
	{
		for (int i = 0; i < maxHp_; i++)
		{
			float posX = drawPos.x + i * iconWidth_;
			float posY = drawPos.y;

			DrawRotaGraph(static_cast<int>(posX), static_cast<int>(posY), static_cast<double>(size_), 0.0, imgHeartLost_, true);

			if (i > hp_ - 1)
			{
				DrawRotaGraph(static_cast<int>(posX), static_cast<int>(posY), static_cast<double>(hps_[i].scale), 0.0, imgHeart_, true);
			}
			else
			{
				DrawRotaGraph(static_cast<int>(posX), static_cast<int>(posY), static_cast<double>(size_), 0.0, imgHeart_, true);
			}
		}
	}
}

void HpManager::Release(void)
{
	// ２Ｄ画像のメモリ解放
	DeleteGraph(imgHeart_);
	DeleteGraph(imgHeartLost_);

	hps_.clear();
}

void HpManager::SetHp(int newHp)
{
	hp_ = newHp;
}

void HpManager::InitHp(void)
{
	for (int i = 0; i < maxHp_; i++)
	{
		HP hp;

		hp.x = i * iconWidth_ + static_cast<int>(pos_.x);
		hp.y = static_cast<int>(pos_.y);
		hp.z = static_cast<int>(pos_.z);
		hp.scale = size_;
		hp.isAlive = true;

		// 可変長配列に追加
		hps_.push_back(hp);
	}
}