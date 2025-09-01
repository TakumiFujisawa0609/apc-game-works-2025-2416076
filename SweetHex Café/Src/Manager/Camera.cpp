#include "Camera.h"
#include "InputManager.h"
#include "../Utility/Utility.h"]
#include "../Object/Player.h"

Camera::Camera(void)
{
	pos_ = {0,0,0};
	angles_ = { 0,0,0 };
}

Camera::~Camera(void)
{
}

void Camera::Init(void)
{
	pos_ = DEFAULT_POS;
	angles_ = DEFAULT_ANGLES;
}

void Camera::Update(void)
{
	if (player_ != nullptr) {
		VECTOR playerPos = player_->GetPos();
		pos_.x = playerPos.x;
		pos_.z = playerPos.z - 1000.0f;
	}
}

void Camera::SetBeforeDraw(void)
{
	//カメラの手前クリップ距離と奥クリップ距離
	SetCameraNearFar(CAMERA_NEAR, CAMERA_FAR);

	//カメラの設定
	SetCameraPositionAndAngle(pos_, angles_.x, angles_.y, angles_.z);
}

void Camera::DrawDebug(void)
{
#ifdef _DEBUG

	DrawFormatString(0, 60, 0xffffff, "カメラ座標 : %.2f,%.2f,%.2f", pos_.x, pos_.y, pos_.z);
	DrawFormatString(0, 80, 0xffffff, "カメラ角度 : %.2f,%.2f,%.2f", angles_.x *180.0f / DX_PI_F, angles_.y * 180.0f / DX_PI_F, angles_.z* 180.0f/DX_PI_F);


#endif // _DEBUG
}

void Camera::Release(void)
{
}

void Camera::SetFollow(Player* player)
{
	player_ = player;
}
