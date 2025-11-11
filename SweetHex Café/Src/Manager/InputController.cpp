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
	if (InputManager::GetInstance().IsNew(KEY_INPUT_W) || InputManager::GetInstance().IsNew(KEY_INPUT_UP))
	{
		return true;
	}

	return false;
}

bool InputController::IsMoveDown(void)
{
	if (InputManager::GetInstance().IsNew(KEY_INPUT_S) || InputManager::GetInstance().IsNew(KEY_INPUT_DOWN))
	{
		return true;
	}

	return false;
}

bool InputController::IsMoveLeft(void)
{
	if (InputManager::GetInstance().IsNew(KEY_INPUT_A) || InputManager::GetInstance().IsNew(KEY_INPUT_LEFT))
	{
		return true;
	}

	return false;
}

bool InputController::IsMoveRight(void)
{
	if (InputManager::GetInstance().IsNew(KEY_INPUT_D) || InputManager::GetInstance().IsNew(KEY_INPUT_RIGHT))
	{
		return true;
	}

	return false;
}

bool InputController::IsSelectUp(void)
{
	if (InputManager::GetInstance().IsTrgDown(KEY_INPUT_W) || InputManager::GetInstance().IsTrgDown(KEY_INPUT_UP))
	{
		return true;
	}

	return false;
}

bool InputController::IsSelectDown(void)
{
	if (InputManager::GetInstance().IsTrgDown(KEY_INPUT_S) || InputManager::GetInstance().IsTrgDown(KEY_INPUT_DOWN))
	{
		return true;
	}

	return false;
}

bool InputController::IsSelectLeft(void)
{
	if (InputManager::GetInstance().IsTrgDown(KEY_INPUT_A) || InputManager::GetInstance().IsTrgDown(KEY_INPUT_LEFT))
	{
		return true;
	}

	return false;
}

bool InputController::IsSelectRight(void)
{
	if (InputManager::GetInstance().IsTrgDown(KEY_INPUT_D) || InputManager::GetInstance().IsTrgDown(KEY_INPUT_RIGHT))
	{
		return true;
	}

	return false;
}

bool InputController::IsPause(void)
{
	if (InputManager::GetInstance().IsTrgDown(KEY_INPUT_ESCAPE) || InputManager::GetInstance().IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::START))
	{
		return true;
	}

	return false;
}

bool InputController::IsDash(void)
{
	if (InputManager::GetInstance().IsNew(KEY_INPUT_LSHIFT) || InputManager::GetInstance().IsPadBtnNew(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::L_TRIGGER))
	{
		return true;
	}

	return false;
}

bool InputController::IsAttack(void)
{
	if (InputManager::GetInstance().IsTrgDown(KEY_INPUT_J) || InputManager::GetInstance().IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::R_TRIGGER))
	{
		SoundManager::GetInstance()->Play(SoundManager::SE::PUNCH);

		return true;
	}

	return false;
}

bool InputController::IsUse(void)
{
	if (InputManager::GetInstance().IsTrgDown(KEY_INPUT_F) || InputManager::GetInstance().IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::DOWN))
	{
		return true;
	}

	return false;
}

bool InputController::IsDecide(void)
{
	if (InputManager::GetInstance().IsTrgDown(KEY_INPUT_SPACE) || InputManager::GetInstance().IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::DOWN))
	{
		return true;
	}

	return false;
}

InputController::InputController(void)
{
}
