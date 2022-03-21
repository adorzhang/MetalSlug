#pragma once
#include <Windows.h>
#include "Constant.h"
class Camera
{
	static Camera* instance;
	float camx, camy;

	Camera();
	~Camera();
public:

	static Camera* GetInstance();

	void SetCamPos(float x, float y) { camx = x; camy = y; }
	float GetCamx() { return camx; }
	float GetCamy() { return camy; }
	void GetCenter(int& _x, int& _y);

};

