#include "Camera.h"
#include "Game.h"
#include "Utils.h"

CCamera::CCamera()
{
	position = VectorInfinity();
}

CCamera::~CCamera()
{
}

Vector2 CCamera::WorldToScreenPoint(Vector2 pos)
{
	Vector3 p = Vector3(0, 0, 0);

	D3DXMATRIX mat;
	D3DXMatrixIdentity(&mat);

	// Translate
	D3DXMATRIX translate;
	D3DXMatrixTranslation(&translate, (pos.x - position.x), (-pos.y + position.y), 0.0f);

	mat *= translate;

	return Vector2((int)mat._41, (int)mat._42);
}

Rect CCamera::GetBoundingBox()
{
	Rect boundingBox;

	if (target != nullptr)
	{
		Vector2 nextPos = position + target->GetVelocity() * CGame::GetDeltaTime();

		boundingBox.left = min(position.x, nextPos.x);
		boundingBox.top = max(position.y, nextPos.y);
		boundingBox.right = max(position.x + bbSize.x, nextPos.x + bbSize.x);
		boundingBox.bottom = min(position.y - bbSize.y, nextPos.y - bbSize.y);
	}
	else
	{
		boundingBox.left = position.x;
		boundingBox.top = position.y;
		boundingBox.right = position.x + bbSize.x;
		boundingBox.bottom = position.y - bbSize.y;
	}

	return boundingBox;
}

void CCamera::PreUpdateSwitchingScrollingSection(Vector2 destination, Vector2 translation)
{
	position += translation;

	if (destination.y < position.y)
	{
		state = CameraState::Switching_BlockBottom;
		velocitySwitching.y = -bbSize.y / switchingDuration * 3;
		boundary_blocking = destination.y - bbSize.y;
	}
	else if (destination.y > position.y)
	{
		state = CameraState::Switching_BlockTop;
		velocitySwitching.y = bbSize.y / switchingDuration * 3;
		boundary_blocking = destination.y;
	}
	else
	{
		state = CameraState::Switching_NoneBlock;
		velocitySwitching.y = 0;
	}

	boundary_stopping = destination.x;

	if (destination.x < position.x)
	{
		velocitySwitching.x = -bbSize.x / switchingDuration;
		isLeftToRight = false;
	}
	else
	{
		velocitySwitching.x = bbSize.x / switchingDuration;
		isLeftToRight = true;
	}
}

void CCamera::PreUpdateSwitchingTopdownSection(Vector2 destination, Vector2 translation)
{
	position += translation;

	boundary_stopping = destination.y;

	velocitySwitching.x = 0;

	if (destination.y < position.y)
	{
		state = CameraState::Switching_StopBottom;
		velocitySwitching.y = -bbSize.y / switchingDuration;
	}
	else if (destination.y > position.y)
	{
		state = CameraState::Switching_StopTop;
		velocitySwitching.y = bbSize.y / switchingDuration;
	}
}

void CCamera::Update()
{
	switch (state)
	{
	case CameraState::FreePlaying_Scrolling:
	case CameraState::FreePlaying_TopDown:
		UpdateFreePlaying();
		break;
	case CameraState::Switching_BlockTop:
	case CameraState::Switching_BlockBottom:
	case CameraState::Switching_NoneBlock:
		UpdateSwitchingScrollingSection();
		break;
	case CameraState::Switching_StopTop:
	case CameraState::Switching_StopBottom:
		UpdateSwitchingTopdownSection();
		break;
	default:
		break;
	}
}

void CCamera::UpdateFreePlaying()
{
	if (target == nullptr) return;

	Vector2 posTarget = target->GetPosition();

	Vector2 vpPlayer = WorldToScreenPoint(posTarget);

	if (position == VectorInfinity() || state == CameraState::FreePlaying_TopDown)
	{
		position.x = (int)(posTarget.x - bbSize.x / 2);
		position.y = (int)(posTarget.y + bbSize.y / 2);
	}
	else
	{
		// Freeze camera
		if (vpPlayer.x <= freezeBoundary.left)
			position.x = (int)(posTarget.x - freezeBoundary.left);
		if (vpPlayer.x >= freezeBoundary.right)
			position.x = (int)(posTarget.x - freezeBoundary.right);

		if (vpPlayer.y <= freezeBoundary.top)
			position.y = (int)(posTarget.y + freezeBoundary.top);
		if (vpPlayer.y >= freezeBoundary.bottom)
			position.y = (int)(posTarget.y + freezeBoundary.bottom);
	}

	// Boundary block
	if (position.x <= boundary.left)
		position.x = boundary.left;
	if (position.x + bbSize.x >= boundary.right)
		position.x = boundary.right - bbSize.x;

	if (position.y >= boundary.top)
		position.y = boundary.top;
	if (position.y - bbSize.y <= boundary.bottom)
		position.y = boundary.bottom + bbSize.y;
}

void CCamera::UpdateSwitchingScrollingSection()
{
	position += velocitySwitching * CGame::GetDeltaTime();

	auto currentScene = (CPlayScene*)CGame::GetInstance()->GetSystem<CSceneManager>()->GetCurrentScene();

	if (state == CameraState::Switching_BlockTop)
	{
		if (position.y >= boundary_blocking) position.y = boundary_blocking;
	}
	else if (state == CameraState::Switching_BlockBottom)
	{
		if (position.y - bbSize.y <= boundary_blocking) position.y = boundary_blocking + bbSize.y;
	}

	if (isLeftToRight == true)
	{
		if (position.x >= boundary_stopping)
		{
			position.x = boundary_stopping;
			state = CameraState::FreePlaying_Scrolling;
			currentScene->AfterSwitchingSection();
		}
	}
	else
	{
		if (position.x <= boundary_stopping)
		{
			position.x = boundary_stopping;
			state = CameraState::FreePlaying_Scrolling;
			currentScene->AfterSwitchingSection();
		}
	}
}

void CCamera::UpdateSwitchingTopdownSection()
{
	position += velocitySwitching * CGame::GetDeltaTime();

	auto currentScene = (CPlayScene*)CGame::GetInstance()->GetSystem<CSceneManager>()->GetCurrentScene();

	if (state == CameraState::Switching_StopTop)
	{
		if (position.y >= boundary_stopping)
		{
			position.y = boundary_stopping;
			state = CameraState::FreePlaying_TopDown;
			currentScene->AfterSwitchingSection();
		}
	}
	else if (state == CameraState::Switching_StopBottom)
	{
		if (position.y <= boundary_stopping)
		{
			position.y = boundary_stopping;
			state = CameraState::FreePlaying_TopDown;
			currentScene->AfterSwitchingSection();
		}
	}

	// Boundary block
	if (position.x <= boundary.left)
		position.x = boundary.left;
	if (position.x + bbSize.x >= boundary.right)
		position.x = boundary.right - bbSize.x;
}
