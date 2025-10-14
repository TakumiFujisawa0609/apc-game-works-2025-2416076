#include "SoundManager.h"
#include <DxLib.h>

SoundManager* SoundManager::instance_ = nullptr;

void SoundManager::Init(void)
{
	////サウンドファイルのリスト作成
	//const char* filename[static_cast<int>(BGM::MAX)]
	//{
	//};

	//// すべてのサウンドの読み込み
	//for (int i = 0; i < static_cast<int>(BGM::MAX); i++)
	//{
	//	bgmHandle_[i] = LoadSoundMem(filename[i]);
	//}

	//サウンドファイルのリスト作成
	const char* sename[static_cast<int>(SE::MAX)]
	{
		"Data/Sound/SE/AS_1424156_かわいい系モンスターのパンチ音.mp3",
	};


	// すべてのサウンドの読み込み
	for (int i = 0; i < static_cast<int>(SE::MAX); i++)
	{
		seHandle_[i] = LoadSoundMem(sename[i]);
	}
}

void SoundManager::Delete(void)
{
	//int bgmMax = static_cast<int>(BGM::MAX);

	//for (int i = 0; i < bgmMax; i++)
	//{
	//	DeleteSoundMem(bgmHandle_[i]);
	//}

	int seMax = static_cast<int>(SE::MAX);

	for (int i = 0; i < seMax; i++)
	{
		DeleteSoundMem(seHandle_[i]);
	}

	delete instance_;
}

void SoundManager::Play(BGM bgm, bool flg)
{
	//int handle = bgmHandle_[static_cast<int>(bgm)];

	//// 指定のハンドルが再生中か確認
	//if (CheckSoundMem(handle) == 0)
	//{
	//	PlaySoundMem(bgmHandle_[static_cast<int>(bgm)], DX_PLAYTYPE_LOOP, flg);
	//}

	//ChangeVolumeSoundMem(255 * 70 / 100, bgmHandle_[static_cast<int>(bgm)]);
}

void SoundManager::Play(SE se)
{

	PlaySoundMem(seHandle_[static_cast<int>(se)], DX_PLAYTYPE_BACK);

	ChangeVolumeSoundMem(255 * 40 / 100, seHandle_[static_cast<int>(se)]);
}

void SoundManager::Stop(BGM bgm)
{
	//StopSoundMem(bgmHandle_[static_cast<int>(bgm)]);
}

void SoundManager::Stop(SE se)
{
	StopSoundMem(seHandle_[static_cast<int>(se)]);
}

SoundManager::SoundManager(void)
{
}

SoundManager::SoundManager(const SoundManager& manager)
{
}

SoundManager::~SoundManager(void)
{
}
