#pragma once

#include <unordered_map>

#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>


#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

#include "Scence.h"
#include <unordered_map>

using namespace std;

#define KEYBOARD_BUFFER_SIZE 1024
enum TYPE_SCENCE
{
	WORLD,
	OVER_WORLD
};

#define MENU_SCENCE_ID	1000
#define SCENCE_ID_OVERWORLD_END	24
class CGame
{
	static CGame * __instance;
	HWND hWnd;									// Window handle

	LPDIRECT3D9 d3d = NULL;						// Direct3D handle
	LPDIRECT3DDEVICE9 d3ddv = NULL;				// Direct3D device object

	LPDIRECT3DSURFACE9 backBuffer = NULL;		
	LPD3DXSPRITE spriteHandler = NULL;			// Sprite helper library to help us draw 2D image on the screen 

	LPDIRECTINPUT8       di;		// The DirectInput object         
	LPDIRECTINPUTDEVICE8 didv;		// The keyboard device 

	BYTE  keyStates[256];			// DirectInput keyboard state buffer 
	DIDEVICEOBJECTDATA keyEvents[KEYBOARD_BUFFER_SIZE];		// Buffered keyboard data

	LPKEYEVENTHANDLER keyHandler;

	float cam_x = 0.0f;
	float cam_y = 0.0f;

	int screen_width;
	int screen_height; 

	unordered_map<int, LPSCENE> scenes;
	int current_scene; 

	void _ParseSection_SETTINGS(string line);
	void _ParseSection_SCENES(string line);

	
	// next map
	bool isNextMap = false;	// check allow change scene
	bool isPreMap = false;	// check allow change scene
	bool isUpMap = false;	// check allow change scene
	bool isDownMap = false;	// check allow change scene
	int sceneId;			// sceneId will change
	bool is_rendering_next_map = false; // Kiểm tra có hiệu ứng đang vẽ map kế tiếp không

	int next_portal_id;
	int next_scence_id_effect;
	bool is_entering_overworld;
	float player_x_world;
	float player_y_world;
public:
	void InitKeyboard();
	void SetKeyHandler(LPKEYEVENTHANDLER handler) { keyHandler = handler; }
	void Init(HWND hWnd);
	void Draw(float x, float y, LPDIRECT3DTEXTURE9 texture, int left, int top, int right, int bottom, int alpha = 255, bool flip = false, float sc = 1);
	//bổ sung
	void SetRenderData(D3DXVECTOR2& center, D3DXVECTOR2& translate, D3DXVECTOR2& scaling);
	void DrawWithTransformation(float x, float y, LPDIRECT3DTEXTURE9 texture, int left, int top, int right, int bottom, int alpha = 255, bool flip = false, float sc = 1, float angle = 0);
	void DrawWithoutTransformation(float x, float y, LPDIRECT3DTEXTURE9 texture, int left, int top, int right, int bottom, int alpha = 255, bool flip = false, float sc = 1, float angle = 0);
	void TransformViewPortPosition(float& x, float& y, float l, float t, float r, float b);

	int IsKeyDown(int KeyCode);
	int IsKeyPress(int KeyCode);
	void ProcessKeyboard();

	void Load(LPCWSTR gameFile);
	LPSCENE GetCurrentScene() { return scenes[current_scene]; }
	LPSCENE GetScene(int id) { return scenes[id]; }
	void SwitchScene(int scene_id, int _alive = 2, int _power = 8);

	int GetScreenWidth() { return screen_width; }
	int GetScreenHeight() { return screen_height; }

	bool CheckCollision(float l1, float t1, float r1, float b1, float l2, float t2, float r2, float b2);

	static void SweptAABB(
		float ml,			// move left 
		float mt,			// move top
		float mr,			// move right 
		float mb,			// move bottom
		float dx,			// 
		float dy,			// 
		float sl,			// static left
		float st, 
		float sr, 
		float sb,
		float &t, 
		float &nx, 
		float &ny);

	LPDIRECT3DDEVICE9 GetDirect3DDevice() { return this->d3ddv; }
	LPDIRECT3DSURFACE9 GetBackBuffer() { return backBuffer; }
	LPD3DXSPRITE GetSpriteHandler() { return this->spriteHandler; }

	void SetCamPos(float x, float y) { cam_x = x; cam_y = y; }
	void GetCamPos(float &x, float &y) { x=cam_x; y=cam_y; }

	static CGame * GetInstance();

	//bo sung

	bool GetIsNextMap() { return this->isNextMap; }
	void SetIsNextMap(bool flag) { this->isNextMap = flag; }

	bool GetIsPreMap() { return this->isPreMap; }
	void SetIsPreMap(bool flag) { this->isPreMap = flag; }


	bool GetIsUpMap() { return this->isUpMap; }
	void SetIsUpMap(bool flag) { this->isUpMap = flag; }

	bool GetIsDownMap() { return this->isDownMap; }
	void SetIsDownMap(bool flag) { this->isDownMap = flag; }

	
	//Get, Set scence id tiếp theo
	void SetSceneId(int _sceneId) { this->sceneId = _sceneId; }
	int GetSceneId() { return this->sceneId; }

	void SetRenderingNextMap(int flag) { this->is_rendering_next_map = flag; }
	int GetRenderingNextMap() { return this->is_rendering_next_map; }

	void SetScenceIDRenderingNextMap(int id) { this->next_scence_id_effect = id; }
	int GetScenceIDRenderingNextMap() { return this->next_scence_id_effect; }

	void SetNextPortalId(int _portalId) { this->next_portal_id = _portalId; }
	int GetNextPortalId() { return this->next_portal_id; }

	int GetCurrentScenceID() { return current_scene; }

	void SetParamEnteringOverWorld(bool _is_entering_overworld) { is_entering_overworld = _is_entering_overworld; }
	bool GetParamEnteringOverWorld() { return is_entering_overworld; }
	void SetParamPlayer(float _x, float _y) { player_x_world = _x; player_y_world = _y; }
	float GetParamXPlayer() { return player_x_world;}
	float GetParamYPlayer() { return player_y_world; }
	int RandomItem(int _item);
	~CGame();

	//new
	
};


