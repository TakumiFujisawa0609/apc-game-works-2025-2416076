#include <chrono>
#include <DxLib.h>

#include "../Scene/TitleScene.h"
#include "../Scene/GameScene.h"
#include "../Scene/ResultScene.h"
#include "../Scene/GameOverScene.h"
#include "../Scene/Loading/Loading.h"

#include "Camera.h"
#include "SceneManager.h"

SceneManager* SceneManager::instance_ = nullptr;

void SceneManager::CreateInstance()
{
	if (instance_ == nullptr)
	{
		instance_ = new SceneManager();
	}
	instance_->Init();
}

SceneManager& SceneManager::GetInstance(void)
{
	return *instance_;
}

void SceneManager::Init(void)
{
	// ロード画面生成
	load_ = new Loading();
	load_->Init();
	load_->Load();

	// カメラ機能の初期化
	camera_ = new Camera();
	camera_->Init();

	// 3D関連の初期化
	Init3D();

	// デルタタイム
	preTime_ = std::chrono::system_clock::now();

	ChangeScene(SCENE_ID::TITLE);
}

void SceneManager::Update(void)
{

	if (scene_ == nullptr) return;

	// デルタタイム
	auto nowTime = std::chrono::system_clock::now();
	deltaTime_ = static_cast<float>(
		std::chrono::duration_cast<std::chrono::nanoseconds>(nowTime - preTime_).count() / 1000000000.0);
	preTime_ = nowTime;


	// カメラの更新
	camera_->Update();

	// ロード中
	if (load_->IsLoading())
	{
		// ロード更新
		load_->Update();

		// ロードの更新が終了していたら
		if (load_->IsLoading() == false)
		{
			// ロード後の初期化
			scene_->LoadEnd();
		}
	}
	// 通常の更新処理
	else
	{
		// 各シーンの更新処理
		scene_->Update();
	}

}

void SceneManager::Draw(void)
{
	// ロード中ならロード画面を描画
	if (load_->IsLoading())
	{
		// ロードの描画
		load_->Draw();
	}
	else
	{
		// 描画先グラフィック領域の指定
		// (３Ｄ描画で使用するカメラの設定などがリセットされる)
		SetDrawScreen(DX_SCREEN_BACK);

		// 画面を初期化
		ClearDrawScreen();

		// カメラ設定
		camera_->SetBeforeDraw();

		// 各シーンの描画処理
		scene_->Draw();
	}
}

void SceneManager::Destroy(void)
{

	// シーンの解放
	scene_->Release();
	delete scene_;

	// ロード画面の開放
	load_->Release();
	delete load_;

	// カメラ機能の解放
	camera_->Release();
	delete camera_;

	// インスタンスのメモリ解放
	delete instance_;

}

void SceneManager::ChangeScene(SCENE_ID nextId)
{
	// シーンを変更する
	sceneId_ = nextId;

	// 現在のシーンを解放
	if (scene_ != nullptr)
	{
		scene_->Release();
		delete scene_;
	}

	switch (sceneId_)
	{
	case SCENE_ID::TITLE:
		scene_ = new TitleScene();
		break;
	case SCENE_ID::GAME:
		scene_ = new GameScene();
		break;
	case SCENE_ID::TUTORIAL:
		//scene_ = new TutorialScene();
		break;
	case SCENE_ID::RESULT:
		scene_ = new ResultScene();
		break;
	case SCENE_ID::GAMEOVER:
		scene_ = new GameOverScene();
		break;

	}

	// 読み込み
	load_->StartAsyncLoad();
	scene_->Load();
	load_->EndAsyncLoad();

	ResetDeltaTime();

}

SceneManager::SCENE_ID SceneManager::GetSceneID(void)
{
	return sceneId_;
}

float SceneManager::GetDeltaTime(void) const
{
	//return 1.0f / 60.0f;
	return deltaTime_;
}

Camera* SceneManager::GetCamera(void)
{
	return camera_;
}

SceneManager::SceneManager(void)
{
	scene_ = nullptr;
	load_ = nullptr;
	sceneId_ = SCENE_ID::NONE;

	// デルタタイム
	deltaTime_ = 1.0f / 60.0f;

}

void SceneManager::ResetDeltaTime(void)
{
	deltaTime_ = 0.016f;
	preTime_ = std::chrono::system_clock::now();
}

void SceneManager::Init3D(void)
{
	// 背景色設定
	SetBackgroundColor(230, 230, 250);

	// Zバッファを有効にする
	SetUseZBuffer3D(true);

	// Zバッファへの書き込みを有効にする
	SetWriteZBuffer3D(true);

	// バックカリングを有効にする
	SetUseBackCulling(true);

	// ライトの設定
	SetUseLighting(true);

	// ライトの設定
	ChangeLightTypeDir({ 0.5f, -0.5f, 0.5f });
}