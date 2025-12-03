#pragma once
#include "SceneBase.h"

class SceneManager;

class TitleScene : public SceneBase
{
public:

	enum class STATE
	{
		START,		// ゲーム開始
		EXIT,		// ゲーム終了
		MAX,
	};

	// コンストラクタ
	TitleScene(void);

	// デストラクタ
	~TitleScene(void);

	void Init(void) override;
	void Load(void)	override;
	void LoadEnd(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Release(void) override;

private:

	// 画像
	int imgTitle_;
	int imgStart_;
	int imgQuit_;

	// 状態
	STATE state_;

	// 状態変更
	void ChangeState(STATE state);

	// 各状態の更新
	void UpdateStart(void);
	void UpdateExit(void);

	// 各状態の描画
	void DrawStart(void);
	void DrawExit(void);

	// タイトルメニュー選択
	void SelectTitleMenu(void);
};
