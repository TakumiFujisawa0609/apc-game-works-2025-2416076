#pragma once
#include <DxLib.h>

class MoviePlayer
{
public:

	enum class STATE
	{
		STOP,
		PLAYING,
		END,
		ERROR_END,
	};

	// コンストラクタ
	MoviePlayer(const char* filePath);
	// デストラクタ
	~MoviePlayer(void);
	// 初期化
	void Init(void);
	void Load(void);
	void LoadEnd(void);
	// 更新
	void Update(void);
	// 描画
	void Draw(void);
	// 解放
	void Release(void);

	// 動画の再生開始
	void Play(void);
	// 動画の再生が終了したかを取得
	bool IsPlayEnd(void)const;
	// 動画のロードに成功したかを取得
	bool IsLoadSuccess(void)const;

private:
	int movieHandle_;
	const char* filePath_;
	bool isLoadSuccess_;
	STATE state_;

	void ChangeState(STATE state);

};