#pragma once
#include <vector>

class SoundManager
{
public:
	// BGMの種類
	enum class BGM
	{
		GAME,
		MAX,
	};

	// SEの種類
	enum class SE
	{
		PUNCH, 
		ENTRY,
		PICKUP,
		REGISTER,
		UP,
		DOWN,
		MAX,
	};

	static constexpr int BGM_VOLUME = 70;
	static constexpr int SE_VOLUME = 40;

	// インスタンスを明示的に生成
	static void CreateInstance(void) { if (instance_ == nullptr) { instance_ = new SoundManager(); } };
	// インスタンスの取得
	static SoundManager* GetInstance(void) { if (instance_ == nullptr) { SoundManager::CreateInstance(); }return instance_; };
	// インスタンスの削除
	static void DeleteInstance(void) { if (instance_ != nullptr) { delete instance_; instance_ = nullptr; } };

	void Init(void);
	void Delete(void);

	//再生
	void Play(BGM bgm,bool topPosFlag = true);
	void Play(SE se, bool topPosFlag = true);
	
	//停止
	void StopSound(void);

private:
	static SoundManager* instance_;

	std::vector<int>bgmHandles_;		// BGMを格納する配列
	std::vector<int> seHandles_;		// SEを格納する配列


	// コンストラクタ・デストラクタをprivateにして、
	// 外部から生成出来ない様にする
	SoundManager(void) = default;
	SoundManager(const SoundManager& manager) = delete;
	~SoundManager(void);
};