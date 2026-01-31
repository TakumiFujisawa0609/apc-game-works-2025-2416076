#pragma once
#include "SceneBase.h"

class MoviePlayer;

class MovieScene : public SceneBase
{
public:
	MovieScene(void);
	~MovieScene(void);

	void Init(void) override;
	void Load(void)	override;
	void LoadEnd(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Release(void) override;

private:
	MoviePlayer* moviePlayer_;
};
