#include "../../Application.h"

#include "../../Manager/SoundManager/SoundManager.h"
#include "../../Manager/SceneManager.h"
#include "../../Manager/SystemManager.h"

#include "EnemySlime.h"
#include "../Item/ItemManager.h"
#include "../Order/OrderManager.h"

#include "EnemyManager.h"

EnemyManager::EnemyManager(Player* player, ItemManager* item, Stage* stage, OrderManager* order)
	:
	player_(player),
	stage_(stage),
	item_(item),
	order_(order),
	cntSpawn_(0),
	nextPatternIndex_(0),
	orderCounter_(0)
{
}

EnemyManager::~EnemyManager()
{
}

void EnemyManager::Init(void)
{
	cntSpawn_ = 0;
	orderCounter_ = 0;

	nextPatternIndex_ = static_cast<int>(EnemyBase::PATTERN::PATTERN_1);
}

void EnemyManager::Load(void)
{
	enemyModelIds_.emplace_back(
		MV1LoadModel((Application::PATH_MODEL + "Enemy/Slime.mv1").c_str()));
}

void EnemyManager::LoadEnd(void)
{
	Init();
}

void EnemyManager::Update(void)
{
	if (enemies_.size() < MAX_ENEMYS)
	{
		// 出現カウンタ
		cntSpawn_++;

		if (cntSpawn_ >= SPAWN_INTERVEL_SLIME)
		{
			EnemyBase* newEnemy = new EnemySlime();

			newEnemy->Init(EnemyBase::TYPE::SLIME, enemyModelIds_[0], player_, EnemyBase::PATTERN::REGISTER,stage_);

			// 注文番号を付与
			orderCounter_++;
			newEnemy->SetOrderId(orderCounter_);

			// リストに追加
			enemies_.emplace_back(newEnemy);

			// カウンターをリセット
			cntSpawn_ = 0;
		}
	}

	for (auto enemy : enemies_)
	{
		// レジに到着したら
		if (enemy->IsRegister() && enemy->GetPattern() == EnemyBase::PATTERN::REGISTER)
		{
			if (!enemy->IsOrderAdded())
			{
				SoundManager::GetInstance()->Play(SoundManager::SE::REGISTER);
				order_->AddOrder(enemy);
				enemy->SetOrderAdded(true);

				EnemyBase::PATTERN nextPattern = UnusedPattern();
				enemy->SetPattern(nextPattern);
				break;

			}
		}
	}

	// イテレータを取得
	auto it = enemies_.begin();

	// リストの最後までループ
	while (it != enemies_.end())
	{
		EnemyBase* enemy = *it;

		enemy->Update();

		// 死亡フラグをチェック
		if (!enemy->IsAlive())
		{
			VECTOR dropPos = enemy->GetPos();

			if (item_ != nullptr)
			{
				// 敵が死んだときアイテムをドロップ
				item_->CreateItem(ItemManager::ITEM_ID_DEFAULT, dropPos);
			}

			// 敵のRelease処理を呼び出す
			enemy->Release();
			// メモリを解放
			delete enemy;

			// リストから要素を削除し、次の要素のイテレータを取得
			it = enemies_.erase(it);
		}
		else if (enemy->IsDoor())
		{
			if (enemy->GetPattern() == EnemyBase::PATTERN::DOOR_1 ||
				enemy->GetPattern() == EnemyBase::PATTERN::DOOR_2)
			{
				// 提供できずに敵が帰ったら、通報され警察がプレイヤーを倒しに来る
				SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAMEOVER);
				break;
			}
		}
		else
		{
			// 生存している場合は、次の要素に進む
			++it;
		}

	
	}
}

void EnemyManager::Draw(void)
{
	for (EnemyBase* enemy : enemies_)
	{
		enemy->Draw();
	}
}

void EnemyManager::Release(void)
{
	for (EnemyBase* enemy : enemies_)
	{
		enemy->Release();
		delete enemy;
	}

	for (int id : enemyModelIds_)
	{
		MV1DeleteModel(id);
	}
}

std::vector<EnemyBase*> EnemyManager::GetEnemies(void)
{
	return enemies_;
}

EnemyBase::PATTERN EnemyManager::UnusedPattern(void)
{

	// パターン総数を定義
	const int MAX_PATTERN_COUNT = static_cast<int>(EnemyBase::PATTERN::MAX);

	// 現在使用中のパターンを記録するためのフラグ配列
	std::vector<bool> isPatternUsed(MAX_PATTERN_COUNT, false);

	// フィールドにいる全ての敵が使っているパターンにフラグを立てる
	for (EnemyBase* enemy : enemies_)
	{
		int patternIndex = static_cast<int>(enemy->GetPattern());

		// パターンインデックスが有効範囲内であることを確認
		if (patternIndex >= static_cast<int>(EnemyBase::PATTERN::PATTERN_1) && patternIndex < MAX_PATTERN_COUNT)
		{
			isPatternUsed[patternIndex] = true;
		}
	}

	// PATTERN_1 のインデックスから順にチェックし、未使用のパターンを見つける
	EnemyBase::PATTERN pattern = EnemyBase::PATTERN::PATTERN_1;
	bool foundUnused = false;

	for (int i = static_cast<int>(EnemyBase::PATTERN::PATTERN_1); i < MAX_PATTERN_COUNT; ++i)
	{
		if (!isPatternUsed[i])
		{
			// 見つかった未使用パターンを割り当て
			pattern = static_cast<EnemyBase::PATTERN>(i);
			foundUnused = true;
			break;
		}
	}

	if (!foundUnused)
	{
		pattern = EnemyBase::PATTERN::MAX;
	}
	return pattern;
}
