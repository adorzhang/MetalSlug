#pragma once

#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>

#include "ComponentManager.h"
#include "Transform.h"
#include "KeyEventHandler.h"

#define BACKGROUND_COLOR D3DCOLOR_XRGB(0, 0, 0)

#define MAX_FRAME_RATE 120

class CSprite;
typedef CSprite* LPSPRITE;

class CGame : public CComponentManager
{
	static CGame* instance;
	static DWORD deltaTime;						// The interval in seconds from the last frame to the current one

	HWND hWnd;									// Window handle

	LPDIRECT3D9 d3d = NULL;						// Direct3D handle
	LPDIRECT3DDEVICE9 d3ddv = NULL;				// Direct3D device object

	LPDIRECT3DSURFACE9 backBuffer = NULL;
	LPD3DXSPRITE spriteHandler = NULL;			// Sprite helper library to help us draw 2D images 

	CKeyEventHandler* key_handler;

	int screen_width;
	int screen_height;
public:
	static CGame* GetInstance();
	static DWORD GetDeltaTime() { return deltaTime; }

	void InitDirectX(HWND hWnd);

	void Draw(Vector2 position, int nx, int layer_index,
		LPDIRECT3DTEXTURE9 texture, int left, int top, int right, int bottom,
		float angle = 0, D3DCOLOR color = D3DCOLOR_ARGB(255, 255, 255, 255));
	LPDIRECT3DTEXTURE9 LoadTexture(LPCWSTR texturePath, D3DCOLOR transparentColor);

	LPDIRECT3DDEVICE9 GetDirect3DDevice() { return this->d3ddv; }
	LPDIRECT3DSURFACE9 GetBackBuffer() { return backBuffer; }
	LPD3DXSPRITE GetSpriteHandler() { return this->spriteHandler; }
	int GetScreenWidth() { return screen_width; }
	int GetScreenHeight() { return screen_height; }

	void Update(DWORD dt);
	void Render();
	void Clean();

	void Init(HWND hWnd);
	void Run();
	void End();
};