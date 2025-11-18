#include <DxLib.h>

#include "../../Application.h"

#include "SoundManager.h"


SoundManager* SoundManager::instance_ = nullptr;

void SoundManager::Init(void)
{
	// BGM
	bgmHandles_.emplace_back(
		LoadSoundMem((Application::PATH_DATA + "Sound/BGM/AS_815874_お昼時のカフェをイメージしたBossa.mp3").c_str()));



	// SE
	seHandles_.emplace_back(
		LoadSoundMem((Application::PATH_DATA + "Sound/SE/AS_1424156_かわいい系モンスターのパンチ音.mp3").c_str()));

	seHandles_.emplace_back(
		LoadSoundMem((Application::PATH_DATA + "Sound/SE/AS_970270_喫茶店やBarのカウベル入店・退店音3.mp3").c_str()));

	seHandles_.emplace_back(
		LoadSoundMem((Application::PATH_DATA + "Sound/SE/AS_850615_キャッチ／捕まえる／ぴょこっ.mp3").c_str()));

	seHandles_.emplace_back(
		LoadSoundMem((Application::PATH_DATA + "Sound/SE/AS_828976_【ぴょこ】可愛い生き物が顔を出す時の音.mp3").c_str()));

	seHandles_.emplace_back(
		LoadSoundMem((Application::PATH_DATA + "Sound/SE/AS_63312_レジの音_チーン_03.mp3").c_str()));

	seHandles_.emplace_back(
		LoadSoundMem((Application::PATH_DATA + "Sound/SE/AS_1465120_デバフ.mp3").c_str()));

	seHandles_.emplace_back(
		LoadSoundMem((Application::PATH_DATA + "Sound/SE/AS_1399231_ピコーン.mp3").c_str()));

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

void SoundManager::Play(BGM bgm, bool topPosFlag)
{
	if (bgmHandles_.empty()) return;

	const int index = static_cast<int>(bgm);
	if (index < 0 || index >= bgmHandles_.size())
	{
		return;
	}

	PlaySoundMem(bgmHandles_[static_cast<int>(bgm)], DX_PLAYTYPE_LOOP, static_cast<int>(topPosFlag));
	ChangeVolumeSoundMem(255 * BGM_VOLUME / 100, bgmHandles_[static_cast<int>(bgm)]);
}

void SoundManager::Play(SE se, bool topPosFlag)
{
	if (seHandles_.empty()) return;

	const int index = static_cast<int>(se);
	if (index < 0 || index >= seHandles_.size())
	{
		return;
	}

	PlaySoundMem(seHandles_[static_cast<int>(se)], DX_PLAYTYPE_BACK,static_cast<int>(topPosFlag));
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