#include <DxLib.h>
#include "Manager/InputManager.h"
#include "Manager/SceneManager.h"
#include "Manager/InputController.h"
#include "Manager/SystemManager.h"
#include "Manager/SoundManager/SoundManager.h"
#include "Common/FpsControl.h"
#include "Application.h"

Application* Application::instance_ = nullptr;

const std::string Application::PATH_DATA = "Data/";
const std::string Application::PATH_IMAGE = PATH_DATA + "Image/";
const std::string Application::PATH_MODEL = PATH_DATA + "Model/";
const std::string Application::PATH_EFFECT = PATH_DATA + "Effect/";
const std::string Application::PATH_MAP_DATA = PATH_DATA + "MapData/MapData";

void Application::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = new Application();
	}
	instance_->Init();
}

Application& Application::GetInstance(void)
{
	return *instance_;
}

void Application::Init(void)
{

	// アプリケーションの初期設定
	SetWindowText("SweetHexCafe");

	// ウィンドウサイズ
	SetGraphMode(SCREEN_SIZE_X, SCREEN_SIZE_Y, 32);
	ChangeWindowMode(true);

	// DxLibの初期化
	SetUseDirect3DVersion(DX_DIRECT3D_11);
	isInitFail_ = false;
	if (DxLib_Init() == -1)
	{
		isInitFail_ = true;
		return;
	}

	// 乱数のシード値を設定する
	DATEDATA date;

	// 現在時刻を取得する
	GetDateTime(&date);

	// 乱数の初期値を設定する
	// 設定する数値によって、ランダムの出方が変わる
	SRand(date.Year + date.Mon + date.Day + date.Hour + date.Min + date.Sec);

	// 入力制御初期化
	SetUseDirectInputFlag(true);

	//FPS初期化
	fps_ = new FpsControl;
	fps_->Init();

	InputManager::CreateInstance();
	InputController::CreateInstance();
	SystemManager::CreateInstance();
	SoundManager::CreateInstance();
	SoundManager::GetInstance()->Init();

	// シーン管理初期化
	SceneManager::CreateInstance();

}

void Application::Run(void)
{

	InputManager& inputManager = InputManager::GetInstance();
	SceneManager& sceneManager = SceneManager::GetInstance();

	// ゲームループ
	while (ProcessMessage() == 0 && !isEnd_)
	{
		//フレームレート更新
		if (!fps_->UpdateFrameRate()) continue;

		inputManager.Update();
		sceneManager.Update();

		sceneManager.Draw();


		fps_->CalcFrameRate();
		fps_->DrawFrameRate();
		ScreenFlip();

	}

}

void Application::Destroy(void)
{
	// シーン管理解放
	SceneManager::GetInstance().Destroy();

	// 入力制御解放
	InputController::GetInstance().Destroy();
	InputManager::GetInstance().Destroy();

	// システム管理解放
	SystemManager::GetInstance().Destroy();


	SoundManager::GetInstance()->Delete();

	//フレームレート解放
	delete fps_;

	// DxLib終了
	if (DxLib_End() == -1)
	{
		isReleaseFail_ = true;
		return;
	}

	// インスタンスのメモリ解放
	delete instance_;

}

bool Application::IsInitFail(void) const
{
	return isInitFail_;
}

bool Application::IsReleaseFail(void) const
{
	return isReleaseFail_;
}

void Application::SetEnd(bool isEnd)
{
	isEnd_ = isEnd;
}

Application::Application(void)
{
	isInitFail_ = false;
	isReleaseFail_ = false;
	isEnd_ = false;
}
