#include "Camera.h"

Camera* Camera::instance = NULL;

Camera* Camera::GetInstance()
{
	if (instance == NULL)
		instance = new Camera();
	return instance;
}

void Camera::GetCenter(int& _x, int& _y)
{
	_x = camx + float(SCREEN_WIDTH) / 2;
	_y = camy + float(SCREEN_HEIGHT) / 2;

}

Camera::Camera()
{
	camx = camy = 0;
}

Camera::~Camera() {}
