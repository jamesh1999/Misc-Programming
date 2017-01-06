#ifndef __CAMERA_INCLUDED__
#define __CAMERA_INCLUDED__

#include <DirectXMath.h>

class Camera
{
public:
	static Camera* instance;
	DirectX::XMFLOAT2 position;
	float scale;

	DirectX::XMFLOAT2 ScreenToWorldPos(float, float);

	Camera();
};

#endif