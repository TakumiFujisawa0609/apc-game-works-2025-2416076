#pragma once
#include "SceneBase.h"

class SceneManager;
class Grid;
class Player;

class GameScene : public SceneBase
{
public:

	// コンストラクタ
	GameScene(void);

	// デストラクタ
	~GameScene(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Release(void) override;

private:

	Grid* grid_;

	Player* player_;
};
