#pragma once
#include "SceneBase.h"

class GameOverScene : public SceneBase
{
public:

	// コンストラクタ
	GameOverScene(void);
	// デストラクタ
	~GameOverScene(void);

	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);

private:

};
