#include "InputManager.h"
#include "SoundManager/SoundManager.h"

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

	if (ins.IsNew(KEY_INPUT_W) || ins.IsNew(KEY_INPUT_UP))
	{
		return true;
	}

	return false;
}

bool InputController::IsMoveDown(void)
{
	auto& ins = InputManager::GetInstance();

	if (ins.IsNew(KEY_INPUT_S) || ins.IsNew(KEY_INPUT_DOWN))
	{
		return true;
	}

	return false;
}

bool InputController::IsMoveLeft(void)
{
	auto& ins = InputManager::GetInstance();

	if (ins.IsNew(KEY_INPUT_A) || ins.IsNew(KEY_INPUT_LEFT))
	{
		return true;
	}

	return false;
}

bool InputController::IsMoveRight(void)
{
	auto& ins = InputManager::GetInstance();

	if (ins.IsNew(KEY_INPUT_D) || ins.IsNew(KEY_INPUT_RIGHT))
	{
		return true;
	}

	return false;
}

bool InputController::IsSelectUp(void)
{
	auto& ins = InputManager::GetInstance();

	if (ins.IsTrgDown(KEY_INPUT_W) || ins.IsTrgDown(KEY_INPUT_UP))
	{
		return true;
	}

	return false;
}

bool InputController::IsSelectDown(void)
{
	auto& ins = InputManager::GetInstance();

	if (ins.IsTrgDown(KEY_INPUT_S) || ins.IsTrgDown(KEY_INPUT_DOWN))
	{
		return true;
	}

	return false;
}

bool InputController::IsSelectLeft(void)
{
	auto& ins = InputManager::GetInstance();

	if (ins.IsTrgDown(KEY_INPUT_A) || ins.IsTrgDown(KEY_INPUT_LEFT))
	{
		return true;
	}

	return false;
}

bool InputController::IsSelectRight(void)
{
	auto& ins = InputManager::GetInstance();

	if (ins.IsTrgDown(KEY_INPUT_D) || ins.IsTrgDown(KEY_INPUT_RIGHT))
	{
		return true;
	}

	return false;
}

bool InputController::IsPause(void)
{
	auto& ins = InputManager::GetInstance();

	if (ins.IsTrgDown(KEY_INPUT_ESCAPE) || ins.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::START))
	{
		return true;
	}

	return false;
}

bool InputController::IsDash(void)
{
	auto& ins = InputManager::GetInstance();

	if (ins.IsNew(KEY_INPUT_LSHIFT) || ins.IsPadBtnNew(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::L_TRIGGER))
	{
		return true;
	}

	return false;
}

bool InputController::IsAttack(void)
{
	auto& ins = InputManager::GetInstance();

	if (ins.IsTrgDown(KEY_INPUT_J) || ins.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::DOWN))
	{
		SoundManager::GetInstance()->Play(SoundManager::SE::PUNCH);

		return true;
	}

	return false;
}

bool InputController::IsUse(void)
{
	auto& ins = InputManager::GetInstance();

	if (ins.IsTrgDown(KEY_INPUT_F) || ins.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::RIGHT))
	{
		return true;
	}

	return false;
}

bool InputController::IsDecide(void)
{
	auto& ins = InputManager::GetInstance();

	if (ins.IsTrgDown(KEY_INPUT_SPACE) || ins.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::DOWN))
	{
		return true;
	}

	return false;
}

InputController::InputController(void)
{
}
