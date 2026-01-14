#include <Dxlib.h>
#include "../Application.h"
#include "../Utility/Utility.h"

#include "../Manager/SceneManager.h"
#include "../Manager/InputManager.h"
#include "../Manager/InputController.h"
#include "../Manager/SystemManager.h"
#include "../Manager/SoundManager/SoundManager.h"
#include "../Manager/Pause.h"

#include "../Object/Player/Player.h"
#include "../Object/Player/Inventory.h"
#include "../Object/Timer/Timer.h"
#include "../Object/Enemy/EnemyManager.h"
#include "../Object/Enemy/EnemyBase.h"
#include "../Object/Weapon/WeaponBase.h"
#include "../Object/Item/ItemManager.h"
#include "../Object/Stage/Stage.h"
#include "../Object/Order/OrderManager.h"
#include "../Object/Counter/Counter.h"


#include "GameScene.h"

GameScene::GameScene(void)
	:
	player_(nullptr),
	timer_(nullptr),
	enemyManager_(nullptr),
	item_(nullptr),
	stage_(nullptr),
	orderManager_(nullptr),
	counter_(nullptr),
	pause_(nullptr),
	state_(STATE::MAX),
	speed_(0.0f),
	warningPos_(VGet(0.0f, 0.0f, 0.0f)),
	isWarning_(false)
{
}

GameScene::~GameScene(void)
{
}

void GameScene::Init(void)
{
	stage_->Init();

	pause_->Init();

	timer_->Init();

	item_->Init();

	player_->Init();

	orderManager_->Init();

	enemyManager_->Init();

	counter_->Init();

	isWarning_ = false;
	warningPos_ = { 320.0f, 0.0f, 1600.0f };
	speed_ = 2.0f;

	ChangeState(STATE::GAME);

}

void GameScene::Load(void)
{
	// 生成処理
	stage_ = new Stage();
	pause_ = new Pause();
	timer_ = new Timer();
	item_ = new ItemManager();
	player_ = new Player(item_);
	orderManager_ = new OrderManager();
	enemyManager_ = new EnemyManager(player_, item_, stage_, orderManager_);
	counter_ = new Counter();

	stage_->Load();
	pause_->Load();
	timer_->Load();
	item_->Load();
	player_->Load();
	orderManager_->Load();
	enemyManager_->Load();
}

void GameScene::LoadEnd(void)
{
	stage_->LoadEnd();
	pause_->LoadEnd();
	timer_->LoadEnd();
	item_->LoadEnd();
	player_->LoadEnd();
	orderManager_->LoadEnd();
	enemyManager_->LoadEnd();
	counter_->Init();

	isWarning_ = false;
	warningPos_ = { 320.0f, 0.0f, 1600.0f };
	speed_ = 2.0f;

	ChangeState(STATE::GAME);
}

void GameScene::Update(void)
{
	switch (state_)
	{
	case STATE::GAME:
		UpdateGame();
		break;
	case STATE::PAUSE:
		UpdatePause();
		break;
	}
}

void GameScene::Draw(void)
{
	stage_->Draw();
	enemyManager_->Draw();
	player_->Draw();
	timer_->Draw();
	orderManager_->Draw();
	counter_->DrawDebug();

	if (item_ != nullptr)
	{
		item_->Draw();
	}

	VECTOR screen = ConvWorldPosToScreenPos(warningPos_);

	if (isWarning_)
	{
		DrawString(static_cast<int>(screen.x), static_cast<int>(screen.y), "提供できないよ！", 0xff0000);
	}

	if (state_ == STATE::PAUSE)
	{
		pause_->Draw();
	}

}

void GameScene::Release(void)
{
	enemyManager_->Release();
	delete enemyManager_;
	enemyManager_ = nullptr;

	player_->Release();
	delete player_;
	player_ = nullptr;

	pause_->Release();
	delete pause_;
	pause_ = nullptr;

	timer_->Release();
	delete timer_;
	timer_ = nullptr;

	item_->Release();
	delete item_;
	item_ = nullptr;

	stage_->Release();
	delete stage_;
	stage_ = nullptr;

	orderManager_->Release();
	delete orderManager_;
	orderManager_ = nullptr;
}

void GameScene::ChangeState(STATE state)
{
	state_ = state;
}

void GameScene::UpdateGame(void)
{
	InputController& ins = InputController::GetInstance();

	stage_->Update();
	timer_->Update();

	// 当たり判定
	CollisionEnemy();
	CollisionWeapon();
	CollisionEnemy2Enemy();
	CollisionCounter();

	CollisionFloor();
	CollisionWall();

	enemyManager_->Update();
	orderManager_->Update();
	player_->Update();

	CollisionWall();

	if (isWarning_)
	{
		if (warningPos_.z < WARNING_MAX_POS)
		{
			warningPos_.z += speed_;
		}
		else
		{
			warningPos_.z = 1600.0f;
			isWarning_ = false;
		}
	}

	// ポーズメニューへ
	if (ins.IsPause())
	{
		SoundManager::GetInstance()->StopSound();
		pause_->SetPause(true);
		ChangeState(STATE::PAUSE);
	}

	// タイマーが0になったらクリアに遷移
	if (timer_->GetIsTimeUp())
	{
		SceneManager::GetInstance().ChangeScene(
			SceneManager::SCENE_ID::RESULT);
		return;
	}

	const std::vector<EnemyBase*>& enemys = enemyManager_->GetEnemies();
	for (EnemyBase* enemy : enemys)
	{
		if (enemy->IsDoor())
		{
			if (enemy->GetPattern() == EnemyBase::PATTERN::DOOR_1 ||
				enemy->GetPattern() == EnemyBase::PATTERN::DOOR_2)
			{
				SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAMEOVER);
				return;
			}
		}
	}
}

void GameScene::UpdatePause(void)
{
	pause_->Update();

	if (!pause_->IsPause())
	{
		SoundManager::GetInstance()->Play(SoundManager::BGM::GAME, false);
		ChangeState(STATE::GAME);
	}
}

void GameScene::CollisionEnemy(void)
{
	// エネミーとプレイヤーの衝突判定
	VECTOR playerPos = player_->GetPos();
	// 敵の情報を取得
	std::vector<EnemyBase*> enemys = enemyManager_->GetEnemies();
	for (EnemyBase* enemy : enemys)
	{
		if (!enemy->IsCollisionState())
		{
			continue;
		}

		if (!player_->IsCollisionState())
		{
			continue;
		}

		VECTOR enemyPos = enemy->GetPos();

	
			// 敵とプレイヤーの衝突判定条件を満たしたら、
		if (Utility::IsHitSpheres(playerPos, Player::COLLISION_RADIUS,
			enemyPos, enemy->GetRadius()))
		{

			Utility::AdjustPositionCollision(playerPos, Player::COLLISION_RADIUS,
				enemyPos, enemy->GetRadius());

			enemy->SetPos(enemyPos);

			player_->SetPos(playerPos);

			if (enemy->GetState() == EnemyBase::STATE::ATTACK)
			{
				if (!player_->IsInvincible())
				{
					player_->Damage(1);
				}
			}
		}
	}
}

void GameScene::CollisionWeapon(void)
{

	// 武器の情報
	WeaponBase* useWeapon = player_->GetUseWeapon();

	// 攻撃中（描画されている）なら
	if (useWeapon->IsAlive())
	{
		// 敵の情報を取得
		std::vector<EnemyBase*> enemys = enemyManager_->GetEnemies();
		for (EnemyBase* enemy : enemys)
		{
			if (!enemy->IsCollisionState())
			{
				continue;
			}

			// 敵と武器の当たり判定
			if (Utility::IsHitSpheres(useWeapon->GetPos(), useWeapon->GetCollisionRadius(),
				enemy->GetPos(), enemy->GetRadius()))
			{
				enemy->Damage(1);
				useWeapon->SetAlive(false);

				for (EnemyBase* enemy : enemyManager_->GetEnemies())
				{
					// 警戒状態で攻撃されていない子なら、追跡・攻撃処理へ
					if (enemy->IsNotice() && enemy->GetState() == EnemyBase::STATE::STANDBY)
					{
						enemy->ChangeState(EnemyBase::STATE::ATTACK);
					}
				}
			}
		}

		// 武器とステージのの当たり判定
		MV1_COLL_RESULT_POLY_DIM hits = MV1CollCheck_Sphere
		(
			stage_->GetModelId(),
			-1,
			useWeapon->GetPos(),
			useWeapon->GetCollisionRadius()
		);

		if (hits.HitNum > 0)
		{
			useWeapon->SetAlive(false);
		}
	}
}

void GameScene::CollisionEnemy2Enemy(void)
{
	// 敵の情報を取得
	std::vector<EnemyBase*> enemies = enemyManager_->GetEnemies();

	// 敵同士の総当たり衝突判定と押し出し処理
	for (size_t i = 0; i < enemies.size(); ++i)
	{
		for (size_t j = i + 1; j < enemies.size(); ++j)
		{
			EnemyBase* enemyA = enemies[i];
			EnemyBase* enemyB = enemies[j];

			// 衝突していたら
			if (Utility::IsHitSpheres(enemyA->GetPos(), enemyA->GetRadius(),
				enemyB->GetPos(), enemyB->GetRadius()))
			{

				// 敵の位置を一時的に格納
				VECTOR tempPosA = enemyA->GetPos();
				VECTOR tempPosB = enemyB->GetPos();

				// 位置を補正
				Utility::AdjustPositionCollision(tempPosA, enemyA->GetRadius(),
					tempPosB, enemyB->GetRadius());

				enemyA->SetPos(tempPosA);

				enemyB->SetPos(tempPosB);
			}
		}
	}
}

void GameScene::CollisionCounter(void)
{
	// プレイヤーの情報
	VECTOR playerPos = player_->GetPos();
	float playerRadius = player_->GetCapsuleRadius();

	// カウンターの情報
	VECTOR counterStartCapsulePos = counter_->GetStartCapsulePos();
	VECTOR counterEndCapsulePos = counter_->GetEndCapsulePos();
	float counterRadius = counter_->GetRadius();

	Inventory* inventory = player_->GetInventory();

	if (Utility::IsHitSphereCapsule(playerPos, playerRadius,
		counterStartCapsulePos, counterEndCapsulePos, counterRadius))
	{
		if (InputController::GetInstance().IsUse())
		{
			// 敵の情報を取得
			std::vector<EnemyBase*> enemies = enemyManager_->GetEnemies();

			if (!enemies.empty())
			{
				for (auto& enemy : enemies)
				{
					if (!enemy->IsServed())
					{
						if (enemy->GetPattern() == EnemyBase::PATTERN::PATTERN_1 ||
							enemy->GetPattern() == EnemyBase::PATTERN::PATTERN_2 ||
							enemy->GetPattern() == EnemyBase::PATTERN::PATTERN_3 ||
							enemy->GetPattern() == EnemyBase::PATTERN::PATTERN_4 ||
							enemy->GetPattern() == EnemyBase::PATTERN::PATTERN_5)
						{
							if (inventory->UseItem())
							{
								SoundManager::GetInstance()->Play(SoundManager::SE::SERVED);
								//一番古い敵に提供する
								enemy->SetServed(true);

								break;
							}
							else
							{
								isWarning_ = true;
								SoundManager::GetInstance()->Play(SoundManager::SE::NO_SERVED);
							}
						}
						else
						{
							isWarning_ = true;
							SoundManager::GetInstance()->Play(SoundManager::SE::NO_SERVED);
						}
					}
				}
			}
		}
	}
}

void GameScene::CollisionWall(void)
{
#pragma region プレイヤー
	// プレイヤー
	VECTOR playerPos = player_->GetPos();

	// カプセルの座標
	VECTOR capStartPos = VAdd(playerPos, player_->GetStartCapsulePos());
	VECTOR capEndPos = VAdd(playerPos, player_->GetEndCapsulePos());

	// カプセルとの当たり判定
	MV1_COLL_RESULT_POLY_DIM hits = MV1CollCheck_Capsule
	(
		stage_->GetModelId(),			// ステージのモデルID
		-1,								// ステージ全てのポリゴンを指定
		capStartPos,					// カプセルの上
		capEndPos,						// カプセルの下
		player_->GetCapsuleRadius()		// カプセルの半径
	);

	// 衝突したポリゴン全ての検索
	for (int i = 0; i < hits.HitNum; i++)
	{
		// ポリゴンを1枚に分割
		auto hit = hits.Dim[i];

		// ポリゴン検索を制限
		for (int tryCnt = 0; tryCnt < 10; tryCnt++)
		{
			// 最初の衝突判定で検出した衝突ポリゴン1枚と衝突判定
			int pHit = HitCheck_Capsule_Triangle
			(
				capStartPos,					// カプセルの上
				capEndPos,						// カプセルの下
				player_->GetCapsuleRadius(),	// カプセルの半径
				hit.Position[0],				// ポリゴン1
				hit.Position[1],				// ポリゴン2
				hit.Position[2]					// ポリゴン3
			);

			// カプセルとポリゴンが当たっていたら
			if (pHit)
			{
				VECTOR normal = hit.Normal;

				// ポリゴンの中心
				VECTOR polyCenter = {
					  (hit.Position[0].x + hit.Position[1].x + hit.Position[2].x) / 3.0f,
					  (hit.Position[0].y + hit.Position[1].y + hit.Position[2].y) / 3.0f,
					  (hit.Position[0].z + hit.Position[1].z + hit.Position[2].z) / 3.0f
				};

				// カプセルからポリゴンへ向かうベクトル
				VECTOR toPoly = VSub(polyCenter, playerPos);

				// 内積で裏から当たっているかを確認
				float dot = VDot(normal, toPoly);

				// 裏から当たっていたら法線を反転
				if (dot > 0)
				{
					normal = VScale(normal, -1.0f);
				}

				// 上下方向の衝突は無視
				if (fabsf(normal.y) > 0.5f)
				{
					continue;
				}

				const float pushBack = 0.1f;
				// 座標をポリゴンの法線方向に移動
				playerPos = VAdd(playerPos, VScale(normal, pushBack));
				// 球体の座標も移動
				capStartPos = VAdd(capStartPos, VScale(normal, pushBack));
				capEndPos = VAdd(capEndPos, VScale(normal, pushBack));


				// 複数当たっている可能性があるので再検索
				continue;
			}

		}
	}
	// 検出したポリゴン情報の後始末
	MV1CollResultPolyDimTerminate(hits);

	player_->SetPos(playerPos);
#pragma endregion


#pragma region エネミー
	std::vector<EnemyBase*> enemys = enemyManager_->GetEnemies();
	for (EnemyBase* enemy : enemys)
	{

		// エネミー
		VECTOR enemyPos = enemy->GetPos();

		// カプセルの座標
		VECTOR capStartPos = VAdd(enemyPos, enemy->GetStartCapsulePos());
		VECTOR capEndPos = VAdd(enemyPos, enemy->GetEndCapsulePos());

		// カプセルとの当たり判定
		auto hits = MV1CollCheck_Capsule
		(
			stage_->GetModelId(),			// ステージのモデルID
			-1,								// ステージ全てのポリゴンを指定
			capStartPos,					// カプセルの上
			capEndPos,						// カプセルの下
			enemy->GetCapsuleRadius()		// カプセルの半径
		);

		// 衝突したポリゴン全ての検索
		for (int i = 0; i < hits.HitNum; i++)
		{
			// ポリゴンを1枚に分割
			auto hit = hits.Dim[i];

			// ポリゴン検索を制限
			for (int tryCnt = 0; tryCnt < 10; tryCnt++)
			{
				// 最初の衝突判定で検出した衝突ポリゴン1枚と衝突判定
				int pHit = HitCheck_Capsule_Triangle
				(
					capStartPos,					// カプセルの上
					capEndPos,						// カプセルの下
					enemy->GetCapsuleRadius(),		// カプセルの半径
					hit.Position[0],				// ポリゴン1
					hit.Position[1],				// ポリゴン2
					hit.Position[2]					// ポリゴン3
				);

				// カプセルとポリゴンが当たっていたら
				if (pHit)
				{
					VECTOR normal = hit.Normal;

					// ポリゴンの中心
					VECTOR polyCenter = {
						  (hit.Position[0].x + hit.Position[1].x + hit.Position[2].x) / 3.0f,
						  (hit.Position[0].y + hit.Position[1].y + hit.Position[2].y) / 3.0f,
							(hit.Position[0].z + hit.Position[1].z + hit.Position[2].z) / 3.0f
					};

					// カプセルからポリゴンへ向かうベクトル
					VECTOR toPoly = VSub(polyCenter, enemyPos);

					// 内積で裏から当たっているかを確認
					float dot = VDot(normal, toPoly);

					// 裏から当たっていたら法線を反転
					if (dot > 0)
					{
						normal = VScale(normal, -1.0f);
					}

					// 上下方向の衝突は無視
					if (fabsf(normal.y) > 0.5f)
					{
						continue;
					}

					const float pushBack = 0.1f;
					// 座標をポリゴンの法線方向に移動
					enemyPos = VAdd(enemyPos, VScale(normal, pushBack));
					// 球体の座標も移動
					capStartPos = VAdd(capStartPos, VScale(normal, pushBack));
					capEndPos = VAdd(capEndPos, VScale(normal, pushBack));


					// 複数当たっている可能性があるので再検索
					continue;
				}

			}

		}
		// 検出したポリゴン情報の後始末
		MV1CollResultPolyDimTerminate(hits);

		enemy->SetPos(enemyPos);
	}
#pragma endregion
}

void GameScene::CollisionFloor(void)
{
#pragma region プレイヤー
	// ステージとアクターの衝突
	VECTOR playerPos = player_->GetPos();

	// 線分の上座標
	VECTOR topPos = playerPos;
	topPos.y = playerPos.y + 50.0f;

	// 線分の下座標
	VECTOR downPos = playerPos;
	downPos.y = playerPos.y - 50.0f;

	// ステージのモデルID
	int modelId = stage_->GetModelId();

	// 線分とモデルの衝突判定
	MV1_COLL_RESULT_POLY res =
		MV1CollCheck_Line(modelId, -1, topPos, downPos);

	// モデルと衝突しているか
	if (res.HitFlag)
	{
		player_->CollisionStage(res.HitPosition);
	}
#pragma endregion

#pragma region エネミー
	std::vector<EnemyBase*> enemys = enemyManager_->GetEnemies();
	for (EnemyBase* enemy : enemys)
	{
		// ステージとアクターの衝突
		VECTOR enemyPos = enemy->GetPos();

		// 線分の上座標
		VECTOR topPos = enemyPos;
		topPos.y = enemyPos.y + 100.0f;

		// 線分の下座標
		VECTOR downPos = enemyPos;
		downPos.y = enemyPos.y - 100.0f;

		// ステージのモデルID
		int modelId = stage_->GetModelId();

		// 線分とモデルの衝突判定
		MV1_COLL_RESULT_POLY res =
			MV1CollCheck_Line(modelId, -1, topPos, downPos);

		// モデルと衝突しているか
		if (res.HitFlag)
		{
			enemy->CollisionStage(res.HitPosition);
		}
	}
#pragma endregion
}