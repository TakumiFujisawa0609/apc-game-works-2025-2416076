#pragma once

class Pause
{
public:

	enum class PAUSE
	{
		CONTINUE,		// 続ける
		RETURN_TITLE,	// タイトルに戻る
		EXIT,			// 終了
		MAX,
	};

	Pause(void);
	~Pause(void);

	void Init(void);
	void Load(void);
	void LoadEnd(void);
	void Update(void);
	void Draw(void);
	void Release(void);

	// セッター・ゲッター
	bool IsPause(void);
	void SetPause(bool pause);

private:
	// ポーズメニューの状態
	PAUSE pause_;

	// ポーズ中かどうか
	bool isPause_;

	// 画像
	int imgContinue_;
	int imgReturn_;
	int imgQuit_;

	// ポーズメニューの状態変更
	void ChangePause(PAUSE pause);

	// ポーズメニューの選択
	void SelectPause(void);

	// 各メニューの更新
	void UpdateContinue(void);
	void UpdateReturnTitle(void);
	void UpdateExit(void);

	// 各メニューの描画
	void DrawContinue(void);
	void DrawReturnTitle(void);
	void DrawExit(void);

};

