#pragma once
#include "SceneBase.h"

class ResultScene : public SceneBase
{
public:

	// コンストラクタ
	ResultScene(void);
	// デストラクタ
	~ResultScene(void);

	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);

private:

};

