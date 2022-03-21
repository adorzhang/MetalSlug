#include <iostream>
#include <fstream>

#include "BossScence.h"
#include "Utils.h"
#include "Textures.h"
#include "Sprites.h"
#include "Sound.h"
#include "MenuScence.h"
#define SCENCE_ID_START	1

using namespace std;

CBossScence::CBossScence(int id, LPCWSTR filePath) :
	CScene(id, filePath)
{
	key_handler = new CBossScenceKeyHandler(this);
}

/*
	Load scene resources from scene file (textures, sprites, animations and objects)
	See scene1.txt, scene2.txt for detail format specification
*/

#define SCENE_SECTION_UNKNOWN -1
#define SCENE_SECTION_TEXTURES 2
#define SCENE_SECTION_SPRITES 3
#define SCENE_SECTION_ANIMATIONS 4
#define SCENE_SECTION_ANIMATION_SETS	5
#define SCENE_SECTION_OBJECTS	6
//bổ sung thêm section map
#define SCENE_SECTION_MAP	7
#define SCENE_SECTION_GRID	8


//Map objects
#define OBJECT_TYPE_BRICK	1

//Enemy Boss objects
#define OBJECT_TYPE_BOSS	2
#define OBJECT_TYPE_COUPLING	3
#define OBJECT_TYPE_PINCER	4
//Main character objects
#define OBJECT_TYPE_MAIN_CHARACTER	9

#define OBJECT_TYPE_HUMAN	11

//Hub objects
#define OBJECT_TYPE_POWERHUB	500
#define OBJECT_TYPE_GUNHUB	501

//String
#define MAX_SCENE_LINE 1024


void CBossScence::_ParseSection_TEXTURES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 5) return; // skip invalid lines

	int texID = atoi(tokens[0].c_str());
	wstring path = ToWSTR(tokens[1]);
	int R = atoi(tokens[2].c_str());
	int G = atoi(tokens[3].c_str());
	int B = atoi(tokens[4].c_str());

	CTextures::GetInstance()->Add(texID, path.c_str(), D3DCOLOR_XRGB(R, G, B));
}

void CBossScence::_ParseSection_SPRITES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 6) return; // skip invalid lines

	int ID = atoi(tokens[0].c_str());
	int l = atoi(tokens[1].c_str());
	int t = atoi(tokens[2].c_str());
	int r = atoi(tokens[3].c_str());
	int b = atoi(tokens[4].c_str());
	int texID = atoi(tokens[5].c_str());

	LPDIRECT3DTEXTURE9 tex = CTextures::GetInstance()->Get(texID);
	if (tex == NULL)
	{
		DebugOut(L"[ERROR] Texture ID %d not found!\n", texID);
		return;
	}

	CSprites::GetInstance()->Add(ID, l, t, r, b, tex);
}

void CBossScence::_ParseSection_ANIMATIONS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	LPANIMATION ani = new CAnimation();

	int ani_id = atoi(tokens[0].c_str());
	for (int i = 1; i < tokens.size(); i += 2)	// why i+=2 ?  sprite_id | frame_time  
	{
		int sprite_id = atoi(tokens[i].c_str());
		int frame_time = atoi(tokens[i + 1].c_str());
		ani->Add(sprite_id, frame_time);
	}

	CAnimations::GetInstance()->Add(ani_id, ani);
}

void CBossScence::_ParseSection_ANIMATION_SETS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 2) return; // skip invalid lines - an animation set must at least id and one animation id

	int ani_set_id = atoi(tokens[0].c_str());

	LPANIMATION_SET s = new CAnimationSet();

	CAnimations* animations = CAnimations::GetInstance();

	for (int i = 1; i < tokens.size(); i++)
	{
		int ani_id = atoi(tokens[i].c_str());

		LPANIMATION ani = animations->Get(ani_id);
		s->push_back(ani);
	}

	CAnimationSets::GetInstance()->Add(ani_set_id, s);
}

/*
	Parse a line in section [OBJECTS]
*/
void CBossScence::_ParseSection_OBJECTS(string line)
{
	vector<string> tokens = split(line);

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	if (tokens.size() < 3) return; // skip invalid lines - an object set must have at least id, x, y
	int object_id = atoi(tokens[0].c_str());
	int object_type = atoi(tokens[1].c_str());
	float x = atof(tokens[2].c_str());
	float y = atof(tokens[3].c_str());

	int ani_set_id = atoi(tokens[4].c_str());

	CAnimationSets* animation_sets = CAnimationSets::GetInstance();

	CGameObject* obj = NULL;

	switch (object_type)
	{
		case OBJECT_TYPE_MAIN_CHARACTER:
			if (player != NULL)
			{
				DebugOut(L"[ERROR] Player object was created before!\n");
				return;
			}
			player = new CMainCharacter(x, y);
			player->SetPosition(x, y);
			player->SetAnimationSet(animation_sets->Get(ani_set_id));
			player->Is_Human = true;
			DebugOut(L"[INFO] Player object created!\n");
			return;
			break;
		case OBJECT_TYPE_HUMAN:
		{
			player_human = new CHuman(x, y);
			player_human->SetAnimationSet(animation_sets->Get(ani_set_id));
			if (type_scence == OVER_WORLD)
				player_human->SetLevel(HUMAN_LEVEL_BIG);
			if (player != NULL)
			{
				DebugOut(L"[INFO] Player object has been Created Already!\n");
				player_human->SetPlayerObject(player);
			}
			return;
			break;
		}
		case OBJECT_TYPE_BOSS:
		{
			int item = 0;
			if (tokens.size() > 5)
				item = atoi(tokens[5].c_str());
			obj = new CBoss(x,y,item);
			LPANIMATION_SET ani_set = animation_sets->Get(200);
			obj->SetAnimationItemSet(ani_set);
			if (player_human != NULL)
			{
				DebugOut(L"[INFO] Player human object has been Created Already!\n");
				dynamic_cast<CBoss*>(obj)->SetPlayerObject(player_human);
			}
			break;
		}
		case OBJECT_TYPE_POWERHUB:
		{
			obj = new CPowerHub();
			obj->SetPosition(x, y);
			obj->SetAnimationSet(animation_sets->Get(ani_set_id));
			hub_objects.push_back(obj);
			DebugOut(L"[INFO] PowerHub object created!\n");
			if (player != NULL)
			{
				DebugOut(L"[INFO] Player object has been Created Already!\n");
				dynamic_cast<CPowerHub*>(obj)->SetPlayerObject(player);
			}
			return;
			break;
		}
		case OBJECT_TYPE_GUNHUB:
		{
			obj = new CGunHub();
			obj->SetPosition(x, y);
			obj->SetAnimationSet(animation_sets->Get(ani_set_id));
			hub_objects.push_back(obj);
			DebugOut(L"[INFO] GunHub object created!\n");
			if (player != NULL)
			{
				DebugOut(L"[INFO] Player object has been Created Already!\n");
				dynamic_cast<CGunHub*>(obj)->SetPlayerObject(player);
			}
			return;
			break;
		}
		default:
			DebugOut(L"[ERR] Invalid object type: %d\n", object_type);
			return;
	}
	// General object setup
	obj->SetPosition(x, y);
	obj->SetID(object_id);
	LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);
	obj->SetAnimationSet(ani_set);
	objects.push_back(obj);
}

void CBossScence::_ParseSection_MAP(string line)
{
	vector<string> tokens = split(line);

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	if (tokens.size() < 1) return; // skip invalid lines - an map must have at least path

	wstring path = ToWSTR(tokens[0]);

	tiledMap = new CTiledMap();
	tiledMap->LoadMap(path.c_str());
	CTiledMapSets::GetInstance()->Add(id, tiledMap);


}

void CBossScence::Load(int _alive, int _power)
{
	DebugOut(L"[INFO] Start loading scene resources from : %s \n", sceneFilePath);

	ifstream f;
	f.open(sceneFilePath);

	// current resource section flag
	int section = SCENE_SECTION_UNKNOWN;

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	

		if (line == "[TEXTURES]") { section = SCENE_SECTION_TEXTURES; continue; }
		if (line == "[SPRITES]") {
			section = SCENE_SECTION_SPRITES; continue;
		}
		if (line == "[ANIMATIONS]") {
			section = SCENE_SECTION_ANIMATIONS; continue;
		}
		if (line == "[ANIMATION_SETS]") {
			section = SCENE_SECTION_ANIMATION_SETS; continue;
		}
		if (line == "[OBJECTS]") {
			section = SCENE_SECTION_OBJECTS; continue;
		}
		if (line == "[MAP]") {
			section = SCENE_SECTION_MAP; continue;
		}
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }

		//
		// data section
		//
		switch (section)
		{
		case SCENE_SECTION_TEXTURES: _ParseSection_TEXTURES(line); break;
		case SCENE_SECTION_SPRITES: _ParseSection_SPRITES(line); break;
		case SCENE_SECTION_MAP: _ParseSection_MAP(line); break;
		case SCENE_SECTION_ANIMATIONS: _ParseSection_ANIMATIONS(line); break;
		case SCENE_SECTION_ANIMATION_SETS: _ParseSection_ANIMATION_SETS(line); break;
		case SCENE_SECTION_OBJECTS: _ParseSection_OBJECTS(line); break;
		}
	}

	f.close();
	//Texture for bounding box
	CTextures::GetInstance()->Add(ID_TEX_BBOX, L"textures\\bbox.png", D3DCOLOR_XRGB(255, 255, 255));
	DebugOut(L"[INFO] Done loading boss scence resources %s\n", sceneFilePath);
	if (player != NULL)
	{
		player->SetPower(_power);
		player->SetAlive(_alive);
	}
}


void CBossScence::Update(DWORD dt)
{
	vector<LPGAMEOBJECT> coObjects;
	for (size_t i = 0; i < objects.size(); i++)
	{
		coObjects.push_back(objects[i]);
	}
	for (size_t i = 0; i < objects.size(); i++)
	{
		objects[i]->Update(dt, &coObjects);
		if (dynamic_cast<CBoss*>(objects[i])->GetState() == BOSS_STATE_IDLE)
		{
			if (player_human)
			{
				player_human->IsStartingBossScence = true;
			}
		}
		else if (dynamic_cast<CBoss*>(objects[i])->GetState() == BOSS_STATE_DIE)
		{
			if (player)
			{
				CGame::GetInstance()->SwitchScene(24, player->GetAlive(), player->GetPower());
			}
		}
		else
		{
			if (player_human)
			{
				player_human->IsStartingBossScence = false;
			}
		}
	}
	if (player_human == NULL) return;
	else
	{
		player_human->Update(dt, &coObjects);
	}
	//Update Hub objects
	for (int i = 0; i < hub_objects.size(); i++)
		hub_objects[i]->Update(dt);

	// Update camera to follow main character
	float cx = 0, cy = 0;
	CGame::GetInstance()->SetCamPos(0, cy = CGame::GetInstance()->GetScreenHeight());
}

void CBossScence::Render()
{
	//Vẽ tất cả các object hiện tại nếu thỏa điều kiện
	if (player->GetState() != MAIN_CHARACTER_STATE_NONE_COLLISION)
	{
		for (int i = 0; i < objects.size(); i++)
			objects[i]->Render();
		//Vẽ player object
		player_human->Render();
		if (player->GetPower() < 0 && player->GetState() == MAIN_CHARACTER_STATE_DIE)
		{
			int lives = player->GetAlive();
			if (lives >= 0)
			{
				lives -= 1;
				player->SetAlive(lives);
				ReLoad();
			}
			else
				ReLoad();
		}
		//Vẽ Hub objects
		for (int i = 0; i < hub_objects.size(); i++)
			hub_objects[i]->Render();
	}
}


/*
	Unload current scene
*/
void CBossScence::Unload()
{
	objects.clear();
	player = NULL;
	player_human = NULL;
	hub_objects.clear();
	CSprites::GetInstance()->Clear();
	DebugOut(L"[INFO] Scene %s unloaded! \n", sceneFilePath);
}

void CBossScence::ReLoad()
{
	Sound::getInstance()->StopAll();
	if (player)
	{
		CMenuScence* menu_scence = dynamic_cast<CMenuScence*>(CGame::GetInstance()->GetScene(MENU_SCENCE_ID));
		if (menu_scence)
			menu_scence->SetScenceId(id);
		CGame::GetInstance()->SwitchScene(MENU_SCENCE_ID, player->GetAlive(), 8);
	}
}

void CBossScenceKeyHandler::OnKeyDown(int KeyCode)
{

}
void CBossScenceKeyHandler::OnKeyUp(int KeyCode)
{
	//DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);
	CHuman* player_human = ((CBossScence*)scence)->GetHumanPlayer();
	if (player_human->GetState() == MAIN_CHARACTER_STATE_DIE) return;
	if (player_human->IsStartingBossScence) return;
	switch (KeyCode)
	{
	case DIK_SPACE:
		player_human->SetState(MAIN_CHARACTER_STATE_JUMP);
		break;
	case DIK_A:
		player_human->Reset();
		break;
	case DIK_Z:
		player_human->SetState(MAIN_CHARACTER_STATE_BARREL_FIRE);
		Sound::getInstance()->PlayNew(SOUND_ID_BULLET_FIRE);
		break;
	}
}

void CBossScenceKeyHandler::KeyState(BYTE* states)
{
	CHuman* player_human = ((CBossScence*)scence)->GetHumanPlayer();
	CGame* game = CGame::GetInstance();
	////// disable control key when Mario die 
	if (player_human->GetState() == MAIN_CHARACTER_STATE_DIE) return;
	if (player_human->GetState() == MAIN_CHARACTER_STATE_NONE_COLLISION) return;
	if (player_human->IsStartingBossScence) return;
	if (game->IsKeyDown(DIK_UP))
	{
		player_human->SetState(MAIN_CHARACTER_STATE_UP_BARREL);
	}
	else if (game->IsKeyDown(DIK_DOWN))
	{
		player_human->SetState(MAIN_CHARACTER_STATE_DOWN_BARREL);
	}
	else if (game->IsKeyDown(DIK_RIGHT))
		player_human->SetState(MAIN_CHARACTER_STATE_RUN_RIGHT);
	else if (game->IsKeyDown(DIK_LEFT))
		player_human->SetState(MAIN_CHARACTER_STATE_RUN_LEFT);
	else
	{
		player_human->SetState(MAIN_CHARACTER_STATE_IDLE);
	}
}

