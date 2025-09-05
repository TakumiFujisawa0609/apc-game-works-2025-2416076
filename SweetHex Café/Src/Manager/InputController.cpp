#include "InputManager.h"
#include "InputController.h"

InputController* InputController::instance_ = nullptr;

void InputController::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = new InputController();
	}
}

InputController& InputController::GetInstance(void)
{
	if (instance_ == nullptr)
	{
		InputController::CreateInstance();
	}
	return *instance_;
}

void InputController::Destroy(void)
{
	// インスタンスのメモリ解放
	delete instance_;
}

bool InputController::IsMoveUp(void)
{
	auto& ins = InputManager::GetInstance();

	if (ins.IsNew(KEY_INPUT_W))
	{
		return true;
	}

	return false;
}

bool InputController::IsMoveDown(void)
{
	auto& ins = InputManager::GetInstance();

	if (ins.IsNew(KEY_INPUT_S))
	{
		return true;
	}

	return false;
}

bool InputController::IsMoveLeft(void)
{
	auto& ins = InputManager::GetInstance();

	if (ins.IsNew(KEY_INPUT_A))
	{
		return true;
	}

	return false;
}

bool InputController::IsMoveRight(void)
{
	auto& ins = InputManager::GetInstance();

	if (ins.IsNew(KEY_INPUT_D))
	{
		return true;
	}

	return false;
}

bool InputController::IsDash(void)
{
	auto& ins = InputManager::GetInstance();

	if (ins.IsNew(KEY_INPUT_LSHIFT))
	{
		return true;
	}

	return false;
}

bool InputController::IsAttack(void)
{
	auto& ins = InputManager::GetInstance();

	if (ins.IsNew(KEY_INPUT_SPACE))
	{
		return true;
	}

	return false;
}

bool InputController::IsUse(void)
{
	auto& ins = InputManager::GetInstance();

	if (ins.IsNew(KEY_INPUT_F))
	{
		return true;
	}

	return false;
}

InputController::InputController(void)
{
}
