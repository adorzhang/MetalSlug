#include <iostream>
#include <fstream>

#include "Game.h"
#include "Utils.h"

#include "PlayScence.h"
#include "IntroScence.h"
#include "EndingScence.h"
#include "BossScence.h"
#include "MenuScence.h"
#include "Sound.h"
CGame * CGame::__instance = NULL;

/*
	Initialize DirectX, create a Direct3D device for rendering within the window, initial Sprite library for 
	rendering 2D images
	- hInst: Application instance handle
	- hWnd: Application window handle
*/
void CGame::Init(HWND hWnd)
{
	LPDIRECT3D9 d3d = Direct3DCreate9(D3D_SDK_VERSION);

	this->hWnd = hWnd;									

	D3DPRESENT_PARAMETERS d3dpp;

	ZeroMemory(&d3dpp, sizeof(d3dpp));

	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
	d3dpp.BackBufferCount = 1;

	RECT r;
	GetClientRect(hWnd, &r);	// retrieve Window width & height 

	d3dpp.BackBufferHeight = r.bottom + 1;
	d3dpp.BackBufferWidth = r.right + 1;

	screen_height = r.bottom + 1;
	screen_width = r.right + 1;

	d3d->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp,
		&d3ddv);

	if (d3ddv == NULL)
	{
		OutputDebugString(L"[ERROR] CreateDevice failed\n");
		return;
	}

	d3ddv->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backBuffer);

	// Initialize sprite helper from Direct3DX helper library
	D3DXCreateSprite(d3ddv, &spriteHandler);
	Sound::getInstance()->loadSound(hWnd);
	OutputDebugString(L"[INFO] InitGame done;\n");
}

/*
	Utility function to wrap LPD3DXSPRITE::Draw 
*/
void CGame::Draw(float x, float y, LPDIRECT3DTEXTURE9 texture, int left, int top, int right, int bottom, int alpha,bool flip)
{
	//D3DXMATRIX oldMatrix;
	//spriteHandler->GetTransform(&oldMatrix);
	D3DXVECTOR3 p(x - cam_x, y - cam_y, 0);
	RECT r; 
	r.left = left;
	r.top = top;
	r.right = right;
	r.bottom = bottom;
	float width = right - left;
	float height = bottom - top;
	int scale = 1;
	D3DXVECTOR2 center = D3DXVECTOR2(p.x+(width / 2) * scale, p.y+(height / 2) * scale); 
	//D3DXVECTOR2 translate = D3DXVECTOR2(x, y); 
	D3DXVECTOR2 scaling = D3DXVECTOR2((flip) ? -1 : 1, 1);
	float angle = 0;
	D3DXMATRIX newMatrix;
	D3DXMatrixTransformation2D(
		&newMatrix,
		&center,
		0.0f,
		&scaling,
		NULL,
		angle,
		NULL
	);
	//D3DXMATRIX finalMatrix=newMatrix*oldMatrix;
	spriteHandler->SetTransform(&newMatrix);
	spriteHandler->Draw(texture, &r, NULL, &p, D3DCOLOR_ARGB(alpha, 255, 255, 255));
	//spriteHandler->SetTransform(&oldMatrix);
}
//bổ sung tạm ở đây
void CGame::SetRenderData(D3DXVECTOR2& center, D3DXVECTOR2& translate, D3DXVECTOR2& scaling)
{
	D3DXMATRIX mt;
	D3DXMatrixIdentity(&mt);
	mt._22 = -1;
	mt._41 = -this->cam_x;
	mt._42 = this->cam_y;
	D3DXVECTOR4 curTranslate;
	D3DXVECTOR4 curCenter;

	D3DXVec2Transform(&curCenter, &D3DXVECTOR2(center.x, center.y), &mt);

	D3DXVec2Transform(&curTranslate, &D3DXVECTOR2(translate.x, translate.y), &mt);

	center.x = curCenter.x;
	center.y = curCenter.y;
	translate.x = curTranslate.x;
	translate.y = curTranslate.y;
	
	
	
}
void CGame::DrawWithTransformation(float x, float y, LPDIRECT3DTEXTURE9 texture, int left, int top, int right, int bottom, int alpha, bool flip )
{
	float width = right - left;
	float height = bottom - top;
	int scale = 1;
	D3DXVECTOR2 center= D3DXVECTOR2(flip?(width/2)*scale-width*scale: (width / 2) * scale,(height/2)*scale);
	D3DXVECTOR2 translate= D3DXVECTOR2(flip?x+width*scale:x,y); 
	D3DXVECTOR2 scaling= D3DXVECTOR2((flip) ? -1 : 1, 1);
	float angle=0;
	SetRenderData(center, translate, scaling);
	RECT r;
	r.left = left;
	r.top = top;
	r.right = right;
	r.bottom = bottom;
	D3DXMATRIX matrix;
	

	D3DXMatrixTransformation2D(
		&matrix,
		NULL,
		0.0f,
		&scaling,
		&center,
		angle,
		&translate
	);
	
	spriteHandler->SetTransform(&matrix);
	spriteHandler->Draw(texture, &r, NULL, NULL, D3DCOLOR_ARGB(alpha, 255,255,255));

}
void CGame::DrawWithoutTransformation(float x, float y, LPDIRECT3DTEXTURE9 texture, int left, int top, int right, int bottom, int alpha, bool flip)
{
	RECT r;
	r.left = left;
	r.top = top;
	r.right = right;
	r.bottom = bottom;
	D3DXMATRIX mt;
	D3DXMatrixIdentity(&mt);
	mt._41 = x;
	mt._42 = y;
	spriteHandler->SetTransform(&mt);
	spriteHandler->Draw(texture, &r, NULL, NULL, D3DCOLOR_ARGB(alpha, 255, 255, 255));
}
int CGame::IsKeyDown(int KeyCode)
{
	return (keyStates[KeyCode] & 0x80) > 0;
}
int CGame::IsKeyPress(int KeyCode)
{
	return IsKeyDown(KeyCode) && !(keyEvents[KeyCode].dwData);
}
void CGame::InitKeyboard()
{
	HRESULT
		hr = DirectInput8Create
		(
			(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
			DIRECTINPUT_VERSION,
			IID_IDirectInput8, (VOID**)&di, NULL
		);

	if (hr != DI_OK)
	{
		DebugOut(L"[ERROR] DirectInput8Create failed!\n");
		return;
	}

	hr = di->CreateDevice(GUID_SysKeyboard, &didv, NULL);

	// TO-DO: put in exception handling
	if (hr != DI_OK) 
	{
		DebugOut(L"[ERROR] CreateDevice failed!\n");
		return;
	}

	// Set the data format to "keyboard format" - a predefined data format 
	//
	// A data format specifies which controls on a device we
	// are interested in, and how they should be reported.
	//
	// This tells DirectInput that we will be passing an array
	// of 256 bytes to IDirectInputDevice::GetDeviceState.

	hr = didv->SetDataFormat(&c_dfDIKeyboard);

	hr = didv->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);


	// IMPORTANT STEP TO USE BUFFERED DEVICE DATA!
	//
	// DirectInput uses unbuffered I/O (buffer size = 0) by default.
	// If you want to read buffered data, you need to set a nonzero
	// buffer size.
	//
	// Set the buffer size to DINPUT_BUFFERSIZE (defined above) elements.
	//
	// The buffer size is a DWORD property associated with the device.
	DIPROPDWORD dipdw;

	dipdw.diph.dwSize = sizeof(DIPROPDWORD);
	dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	dipdw.diph.dwObj = 0;
	dipdw.diph.dwHow = DIPH_DEVICE;
	dipdw.dwData = KEYBOARD_BUFFER_SIZE; // Arbitary buffer size

	hr = didv->SetProperty(DIPROP_BUFFERSIZE, &dipdw.diph);

	hr = didv->Acquire();
	if (hr != DI_OK)
	{
		DebugOut(L"[ERROR] DINPUT8::Acquire failed!\n");
		return;
	}

	DebugOut(L"[INFO] Keyboard has been initialized successfully\n");
}

void CGame::ProcessKeyboard()
{
	HRESULT hr; 

	// Collect all key states first
	hr = didv->GetDeviceState(sizeof(keyStates), keyStates);
	if (FAILED(hr))
	{
		// If the keyboard lost focus or was not acquired then try to get control back.
		if ((hr == DIERR_INPUTLOST) || (hr == DIERR_NOTACQUIRED))
		{
			HRESULT h = didv->Acquire();
			if (h==DI_OK)
			{ 
				DebugOut(L"[INFO] Keyboard re-acquired!\n");
			}
			else return;
		}
		else
		{
			//DebugOut(L"[ERROR] DINPUT::GetDeviceState failed. Error: %d\n", hr);
			return;
		}
	}

	keyHandler->KeyState((BYTE *)&keyStates);



	// Collect all buffered events
	DWORD dwElements = KEYBOARD_BUFFER_SIZE;
	hr = didv->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), keyEvents, &dwElements, 0);
	if (FAILED(hr))
	{
		//DebugOut(L"[ERROR] DINPUT::GetDeviceData failed. Error: %d\n", hr);
		return;
	}

	// Scan through all buffered events, check if the key is pressed or released
	for (DWORD i = 0; i < dwElements; i++)
	{
		int KeyCode = keyEvents[i].dwOfs;
		int KeyState = keyEvents[i].dwData;
		if ((KeyState & 0x80) > 0)
			keyHandler->OnKeyDown(KeyCode);
		else
			keyHandler->OnKeyUp(KeyCode);
	}
}

CGame::~CGame()
{
	if (spriteHandler != NULL) spriteHandler->Release();
	if (backBuffer != NULL) backBuffer->Release();
	if (d3ddv != NULL) d3ddv->Release();
	if (d3d != NULL) d3d->Release();
}

/*
	Standard sweptAABB implementation
	Source: GameDev.net
*/
void CGame::SweptAABB(
	float ml, float mt,	float mr, float mb,			
	float dx, float dy,			
	float sl, float st, float sr, float sb,
	float &t, float &nx, float &ny)
{

	float dx_entry, dx_exit, tx_entry, tx_exit;
	float dy_entry, dy_exit, ty_entry, ty_exit;

	float t_entry; 
	float t_exit; 

	t = -1.0f;			// no collision
	nx = ny = 0;

	//
	// Broad-phase test 
	//

	float bl = dx > 0 ? ml : ml + dx;
	float bt = dy > 0 ? mt : mt + dy;
	float br = dx > 0 ? mr + dx : mr;
	float bb = dy > 0 ? mb + dy : mb;

	if (br < sl || bl > sr || bb < st || bt > sb) return;


	if (dx == 0 && dy == 0) return;		// moving object is not moving > obvious no collision

	if (dx > 0)
	{
		dx_entry = sl - mr; 
		dx_exit = sr - ml;
	}
	else if (dx < 0)
	{
		dx_entry = sr - ml;
		dx_exit = sl- mr;
	}


	if (dy > 0)
	{
		dy_entry = st - mb;
		dy_exit = sb - mt;
	}
	else if (dy < 0)
	{
		dy_entry = sb - mt;
		dy_exit = st - mb;
	}

	if (dx == 0)
	{
		tx_entry = -999999.0f;
		tx_exit = 999999.0f;
	}
	else
	{
		tx_entry = dx_entry / dx;
		tx_exit = dx_exit / dx;
	}
	
	if (dy == 0)
	{
		ty_entry = -99999.0f;
		ty_exit = 99999.0f;
	}
	else
	{
		ty_entry = dy_entry / dy;
		ty_exit = dy_exit / dy;
	}
	

	if (  (tx_entry < 0.0f && ty_entry < 0.0f) || tx_entry > 1.0f || ty_entry > 1.0f) return;

	t_entry = max(tx_entry, ty_entry);
	t_exit = min(tx_exit, ty_exit);
	
	if (t_entry > t_exit) return; 

	t = t_entry; 

	if (tx_entry > ty_entry)
	{
		ny = 0.0f;
		dx > 0 ? nx = -1.0f : nx = 1.0f;
	}
	else 
	{
		nx = 0.0f;
		dy > 0?ny = -1.0f:ny = 1.0f;
		
	}

}
bool CGame::CheckCollision(float l1, float t1, float r1, float b1, float l2, float t2, float r2, float b2)
{
	if (t1 <= b2 && b1 >= t2 && l1 <= r2 && r1 >= l2)
		return true;
	return false;
}
CGame *CGame::GetInstance()
{
	if (__instance == NULL) __instance = new CGame();
	return __instance;
}

#define MAX_GAME_LINE 1024


#define GAME_FILE_SECTION_UNKNOWN -1
#define GAME_FILE_SECTION_SETTINGS 1
#define GAME_FILE_SECTION_SCENES 2

#define INTRO_SCENCE_ID 0
#define BOSS_SCENCE_ID 99
#define ENDING_SCENCE_ID 100

void CGame::_ParseSection_SETTINGS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 2) return;
	if (tokens[0] == "start")
		current_scene = atoi(tokens[1].c_str());
	else
		DebugOut(L"[ERROR] Unknown game setting %s\n", ToWSTR(tokens[0]).c_str());
}

void CGame::_ParseSection_SCENES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 2) return;
	int id = atoi(tokens[0].c_str());
	LPCWSTR path = ToLPCWSTR(tokens[1]);
	int type_scence= atoi(tokens[2].c_str());
	LPSCENE scene;
	if(id==INTRO_SCENCE_ID)
		scene = new CIntroScence(id, path);
	else if (id == BOSS_SCENCE_ID)
		scene = new CBossScence(id, path);
	else if(id == ENDING_SCENCE_ID)
		scene = new CEndingScence(id, path);
	else if (id == MENU_SCENCE_ID)
		scene = new CMenuScence(id, path);
	else
		scene = new CPlayScene(id, path);
	if (type_scence == OVER_WORLD)
		scene->SetTypeScence(OVER_WORLD);
	else if((type_scence == WORLD))
		scene->SetTypeScence(WORLD);
	scenes[id] = scene;
}

/*
	Load game campaign file and load/initiate first scene
*/
void CGame::Load(LPCWSTR gameFile)
{
	DebugOut(L"[INFO] Start loading game file : %s\n", gameFile);

	ifstream f;
	f.open(gameFile);
	char str[MAX_GAME_LINE];

	// current resource section flag
	int section = GAME_FILE_SECTION_UNKNOWN;

	while (f.getline(str, MAX_GAME_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	

		if (line == "[SETTINGS]") { section = GAME_FILE_SECTION_SETTINGS; continue; }
		if (line == "[SCENES]") { section = GAME_FILE_SECTION_SCENES; continue; }

		//
		// data section
		//
		switch (section)
		{
			case GAME_FILE_SECTION_SETTINGS: _ParseSection_SETTINGS(line); break;
			case GAME_FILE_SECTION_SCENES: _ParseSection_SCENES(line); break;
		}
	}
	f.close();

	DebugOut(L"[INFO] Loading game file : %s has been loaded successfully\n",gameFile);

	SwitchScene(current_scene);
}

void CGame::SwitchScene(int scene_id, int _alive, int _power)
{
	DebugOut(L"[INFO] Switching to scene %d\n", scene_id);

	scenes[current_scene]->Unload();

	CTextures::GetInstance()->Clear();
	CSprites::GetInstance()->Clear();
	CAnimations::GetInstance()->Clear();
	//bổ sung
	if(CTiledMapSets::GetInstance()->Get(current_scene))
		CTiledMapSets::GetInstance()->Get(current_scene)->Clear();
	current_scene = scene_id;
	LPSCENE s = scenes[scene_id];
	CGame::GetInstance()->SetKeyHandler(s->GetKeyEventHandler());
	s->Load(_alive, _power);
}


void CGame::TransformViewPortPosition(float& x, float& y, float l, float t, float r, float b)
{
	float width = r - l;
	float height = b - t;
	int scale = 1; 
	D3DXVECTOR3 center = D3DXVECTOR3((width / 2) * scale, (height / 2) * scale, 0);
	RECT rect;
	rect.left = l;
	rect.top = t;
	rect.right = r;
	rect.bottom = b;

	D3DXMATRIX mt;
	D3DXMatrixIdentity(&mt);
	mt._22 = -1;
	mt._41 = -this->cam_x;
	mt._42 = this->cam_y;
	D3DXVECTOR3 position = D3DXVECTOR3(x, y, 0);

	D3DXVECTOR4 vp_pos;
	D3DXVec3Transform(&vp_pos, &position, &mt);

	D3DXVECTOR3 pos(vp_pos.x, vp_pos.y, 0);
	x = pos.x;
	y = pos.y;
}

int CGame::RandomItem(int _item)
{
	int itemRandom;

	if (_item > 6)
	{
		int random = rand() % 100;

		if (random < 10)
		{
			itemRandom = 0;// ANI_ITEM_WHIP
		}
		else if (random < 20 && random >= 10)
		{
			itemRandom = 1;// ANI_ITEM_KNIFE
		}
		else if (random < 30 && random >= 20)
		{
			itemRandom = 2;// ANI_ITEM_AXE
		}
		else if (random < 40 && random >= 30)
		{
			itemRandom = 3;// ANI_ITEM_BOMERANG
		}
		else if (random < 50 && random >= 40)
		{
			itemRandom = 6;// ANI_ITEM_FIRE
		}
		else if (random < 60 && random >= 50)
		{
			itemRandom = 4;// ANI_ITEM_SMALL_HEART
		}
		else
		{
			itemRandom = 5;// ANI_ITEM_BIG_HEART
		}
	}
	else
	{
		itemRandom = _item;
	}

	return itemRandom;
}