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

bool InputController::IsUp(void)
{
	auto& ins = InputManager::GetInstance();

	if (ins.IsNew(KEY_INPUT_W) || ins.IsNew(KEY_INPUT_UP))
	{
		return true;
	}

	return false;
}

bool InputController::IsDown(void)
{
	auto& ins = InputManager::GetInstance();

	if (ins.IsNew(KEY_INPUT_S) || ins.IsNew(KEY_INPUT_DOWN))
	{
		return true;
	}

	return false;
}

bool InputController::IsLeft(void)
{
	auto& ins = InputManager::GetInstance();

	if (ins.IsNew(KEY_INPUT_A) || ins.IsNew(KEY_INPUT_LEFT))
	{
		return true;
	}

	return false;
}

bool InputController::IsRight(void)
{
	auto& ins = InputManager::GetInstance();

	if (ins.IsNew(KEY_INPUT_D) || ins.IsNew(KEY_INPUT_RIGHT))
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

	if (ins.IsTrgDown(KEY_INPUT_SPACE))
	{
		return true;
	}

	return false;
}

bool InputController::IsUse(void)
{
	auto& ins = InputManager::GetInstance();

	if (ins.IsTrgDown(KEY_INPUT_F))
	{
		return true;
	}

	return false;
}

bool InputController::IsDecide(void)
{
	auto& ins = InputManager::GetInstance();

	if (ins.IsTrgDown(KEY_INPUT_SPACE))
	{
		return true;
	}

	return false;
}

InputController::InputController(void)
{
}
