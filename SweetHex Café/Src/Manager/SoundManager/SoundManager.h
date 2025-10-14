#pragma once

class SoundManager
{
public:
	// BGMの種類
	enum class BGM
	{

		MAX,
	};

	// SEの種類
	enum class SE
	{
		PUNCH,
		MAX,
	};

	// インスタンスを明示的に生成
	static void CreateInstance(void) { if (instance_ == nullptr) { instance_ = new SoundManager(); } };
	// インスタンスの取得
	static SoundManager* GetInstance(void) { if (instance_ == nullptr) { SoundManager::CreateInstance(); }return instance_; };
	// インスタンスの削除
	static void DeleteInstance(void) { if (instance_ != nullptr) { delete instance_; } };

	void Init(void);
	void Delete(void);

	//再生
	void Play(BGM bgm,bool flg = true);
	void Play(SE se);
	
	//停止
	void Stop(BGM bgm);
	void Stop(SE se);

private:
	static SoundManager* instance_;

	//int bgmHandle_[static_cast<int>(BGM::MAX)];			// BGMを格納する配列
	int seHandle_[static_cast<int>(SE::MAX)];			// SEを格納する配列

	// コンストラクタ・デストラクタをprivateにして、
	// 外部から生成出来ない様にする
	SoundManager(void);
	SoundManager(const SoundManager& manager);
	~SoundManager(void);
};