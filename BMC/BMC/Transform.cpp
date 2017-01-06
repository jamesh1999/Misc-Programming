#include "stdafx.h"
#include "Transform.h"
#include <cmath>

DirectX::XMMATRIX Transform::GetTransform() const
{
	return DirectX::XMMatrixMultiply(
		DirectX::XMMatrixMultiply(
			DirectX::XMMatrixRotationRollPitchYawFromVector(DirectX::XMLoadFloat3(&rotation)),
			DirectX::XMMatrixScalingFromVector(DirectX::XMLoadFloat3(&scale))),
		DirectX::XMMatrixTranslationFromVector(DirectX::XMLoadFloat3(&position)));
}

DirectX::XMVECTOR Transform::GetPosition() const
{
	return DirectX::XMLoadFloat3A(&position);
}

DirectX::XMVECTOR Transform::GetRotation() const
{
	return DirectX::XMLoadFloat3A(&rotation);
}

DirectX::XMVECTOR Transform::GetScale() const
{
	return DirectX::XMLoadFloat3A(&scale);
}

void Transform::SetPosition(const DirectX::XMVECTOR& nPos)
{
	DirectX::XMStoreFloat3A(&position, nPos);
}

void Transform::SetRotation(const DirectX::XMVECTOR& nRotation)
{
	DirectX::XMStoreFloat3A(&rotation, nRotation);
	//Clamp [-pi:pi]
	rotation.x -= DirectX::XM_2PI * std::floor(rotation.x / DirectX::XM_2PI);
	rotation.y -= DirectX::XM_2PI * std::floor(rotation.y / DirectX::XM_2PI);
	rotation.z -= DirectX::XM_2PI * std::floor(rotation.z / DirectX::XM_2PI);
}

void Transform::SetScale(const DirectX::XMVECTOR& nScale)
{
	DirectX::XMStoreFloat3A(&scale, nScale);
}