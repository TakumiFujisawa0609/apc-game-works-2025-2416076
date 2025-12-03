#pragma once
#include "SceneBase.h"

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

};
