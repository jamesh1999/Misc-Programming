#ifndef __TRANSFORM_INCLUDED__
#define __TRANSFORM_INCLUDED__

#include "Component.h"
#include "DirectXMath.h"

class Transform : public Component
{
private:
	DirectX::XMFLOAT3A position;
	DirectX::XMFLOAT3A rotation;
	DirectX::XMFLOAT3A scale;

public:
	DirectX::XMMATRIX GetTransform() const;
	DirectX::XMVECTOR GetPosition() const;
	DirectX::XMVECTOR GetRotation() const;
	DirectX::XMVECTOR GetScale() const;

	void SetPosition(const DirectX::XMVECTOR&);
	void SetRotation(const DirectX::XMVECTOR&);
	void SetScale(const DirectX::XMVECTOR&);
};

#endif