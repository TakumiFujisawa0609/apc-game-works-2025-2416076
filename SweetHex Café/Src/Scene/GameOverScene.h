#pragma once
#include "SceneBase.h"
#include <DxLib.h>

class GameOverScene : public SceneBase
{
public:

	// コンストラクタ
	GameOverScene(void);
	// デストラクタ
	~GameOverScene(void);

	void Init(void)override;
	void Load(void)	override;
	void LoadEnd(void) override;
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

	int gameOverImg_;
	int pushImg_;
	int spaceKeyImg_;
	int aButtonImg_;
	float scale_;
	bool isScale_;
};
