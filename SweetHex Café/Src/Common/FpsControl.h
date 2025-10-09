#pragma once

//フレームレート
static constexpr float FRAME_RATE(1000 / 60);

class FpsControl
{
public:
	//コンストラクタ・デストラクタ
	FpsControl();
	~FpsControl();

	void Init(void);				//初期化
	bool UpdateFrameRate(void);		//フレームレート更新
	void CalcFrameRate(void);		//フレームレート計算
	void DrawFrameRate(void);		//フレームレート表示

private:
	int currentTime_;				//現在の時間
	int prevFrameTime_;				//前回のフレーム実行時の時間

	int frameCnt_;					//フレームカウント
	int updateFrameRateTime_;		//フレームレートを更新した時間

	float frameRate_;				//フレームレート
};