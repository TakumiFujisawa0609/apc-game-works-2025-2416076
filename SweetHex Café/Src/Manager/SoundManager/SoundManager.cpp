#include <DxLib.h>

#include "../../Application.h"

#include "SoundManager.h"


SoundManager* SoundManager::instance_ = nullptr;

void SoundManager::Init(void)
{
	// BGM	

	// SE
	seHandles_.emplace_back(
		LoadSoundMem((Application::PATH_DATA + "Sound/SE/AS_1424156_かわいい系モンスターのパンチ音.mp3").c_str()));

}

void SoundManager::Delete(void)
{
	for (int bgm : bgmHandles_)
	{
		DeleteSoundMem(bgm);
	}

	for (int se : seHandles_)
	{
		DeleteSoundMem(se);
	}

	bgmHandles_.clear();
	seHandles_.clear();
}

void SoundManager::Play(BGM bgm, bool loop)
{
	if (bgmHandles_.empty()) return;

	const int index = static_cast<int>(bgm);
	if (index < 0 || index >= bgmHandles_.size())
	{
		return;
	}

	PlaySoundMem(bgmHandles_[static_cast<int>(bgm)], DX_PLAYTYPE_LOOP, loop);
	ChangeVolumeSoundMem(255 * BGM_VOLUME / 100, bgmHandles_[static_cast<int>(bgm)]);
}

void SoundManager::Play(SE se)
{
	if (seHandles_.empty()) return;

	const int index = static_cast<int>(se);
	if (index < 0 || index >= seHandles_.size())
	{
		return;
	}

	PlaySoundMem(seHandles_[static_cast<int>(se)], DX_PLAYTYPE_BACK);
	ChangeVolumeSoundMem(255 * SE_VOLUME / 100, seHandles_[static_cast<int>(se)]);
}

void SoundManager::StopSound(void)
{
	for (int bgm : bgmHandles_)
	{
		StopSoundMem(bgm);
	}

	for (int se : seHandles_)
	{
		StopSoundMem(se);
	}
}

SoundManager::~SoundManager(void)
{
}