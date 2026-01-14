#pragma once
#include "SceneBase.h"
#include <DxLib.h>


class AnimationController;

class ResultScene : public SceneBase
{
public:

	// コンストラクタ
	ResultScene(void);
	// デストラクタ
	~ResultScene(void);

	void Init(void)override;
	void Load(void)override;
	void LoadEnd(void)override;
	void Update(void)override;
	void Draw(void)override;
	void Release(void)override;

private:

	AnimationController* animationController_;

	int modelId_;

	VECTOR pos_;
	VECTOR scales_;
	VECTOR angles_;
	VECTOR localAngles_;

	int gameClearImg_;
	int pushImg_;
	int spaceKeyImg_;
	int aButtonImg_;
	float scale_;
	bool isScale_;
};

