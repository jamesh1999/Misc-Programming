#include "stdafx.h"
#include "Camera.h"

Camera* Camera::instance = nullptr;

Camera::Camera()
{
	instance = this;
}

DirectX::XMFLOAT2 Camera::ScreenToWorldPos(float x, float y)
{
	return DirectX::XMFLOAT2((x / scale) + position.x, (y / scale) + position.y);
}
