#pragma once
#include "Game.h"
#include "Textures.h"
#include "Scence.h"
#include "GameObject.h"
//new - add a camera class
#include "Camera.h"

#include "Brick.h"
#include "Lava.h"
#include "Stair.h"
#include "Mario.h"
#include "Goomba.h"
#include "Koopas.h"
#include "EnemyObject1.h"
#include "Worm.h"
#include "Spider.h"
#include "Cannon.h"
#include "Spike.h"
#include "Eyeball.h"
#include "Teleporter.h"
#include "Floater.h"
#include "Dome.h"
#include "Jumper.h"
#include "Insect.h"
#include "SKull.h"
#include "Orb.h"
#include "Item.h"
#include "Mine.h"
#include "Egg.h"
#include "Arrow.h"
//bo sung
#include "MainCharacter.h"
#include "BarrelObject.h"
#include "Legs.h"
#include "CabinObject.h"
#include "Human.h"
#include "Vehicle.h"

#include "TiledMap.h"
#include "Grid.h"
#include "SpecialPortal.h"

#define MENU_SCENCE_ID	1000
#define START_SCENCE_ID	1
class CPlayScene : public CScene
{
protected:
	//CMario *player;	
	CMainCharacter* player;// A play scene has to have player, right? 
	CHuman* player_human;
	float cx,cy;
	float cam_y;
	vector<LPGAMEOBJECT> objects;
	unordered_map<int,LPGAMEOBJECT> map_objects;

	vector<LPGAMEOBJECT> hub_objects;

	void _ParseSection_TEXTURES(string line);
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_ANIMATION_SETS(string line);
	void _ParseSection_OBJECTS(string line);
	//Bổ sung map
	void _ParseSection_MAP(string line);
	void _ParseSection_GRID(string line);

	CMap* tiledMap;
	int id_next_map = -1;
	int id_pre_map = -1;
	LPCWSTR tiled_map_file_path_next;
	CSprites* sprites_next_map;

	bool isMenuScenceDisplayed;

	Camera* gameCamera;// new

public:
	bool isRenderNextMap = false;
	bool isRenderPreMap = false;
	bool initNextMap = true;
	bool initGridFlag = true;
	CPlayScene(int id, LPCWSTR filePath);
	virtual void Load() {};
	virtual void Load(int _alive = 2, int _power = 0);
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();

	//CMario * GetPlayer() { return player; } 
	CMainCharacter* GetPlayer() { return player; }
	CHuman* GetHumanPlayer() { return player_human; }

	//friend class CPlayScenceKeyHandler;

	virtual CMap* GetMap();
	virtual void GetNextMap();


	CSprites* GetSpritesNextMap() { return sprites_next_map; }

	void ReLoad();
};

class CPlayScenceKeyHandler : public CScenceKeyHandler
{
public:
	virtual void KeyState(BYTE* states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);
	CPlayScenceKeyHandler(CScene* s) :CScenceKeyHandler(s) {};
};

