#pragma once
#include "SceneBase.h"

class SceneManager;
class Grid;
class BlockManager;
class Player;
class Pause;
class Timer;
class Stage;
class EnemyManager;
class ItemManager;

class GameScene : public SceneBase
{
public:

	enum class STATE
	{
		GAME,
		PAUSE,
		MAX,
	};

	// コンストラクタ
	GameScene(void);

	// デストラクタ
	~GameScene(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Release(void) override;

private:
	// ゲームの状態
	STATE state_;

	// 各オブジェクト
	Grid* grid_;
	//BlockManager* blockManager_;
	Player* player_;
	Pause* pause_;
	Timer* timer_;
	EnemyManager* enemyManager_;
	ItemManager* item_;
	Stage* stage_;


	// 状態変更
	void ChangeState(STATE state);

	// 更新処理
	void UpdateGame(void);
	void UpdatePause(void);

	void CollisionEnemy(void);
	void CollisionWeapon(void);
	void CollisionEnemy2Enemy(void);

	void CollisionWall(void);
	void CollisionFloor(void);
};
