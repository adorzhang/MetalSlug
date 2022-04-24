#include <iostream>
#include <fstream>

#include "MenuScence.h"
#include "Utils.h"
#include "Textures.h"
#include "Sprites.h"

#define START_SCENCE_ID	1

using namespace std;

bool isContinueButton = true;
bool isCancelButton = false;
bool isEnter = false;
CMenuScence::CMenuScence(int id, LPCWSTR filePath) :
	CScene(id, filePath)
{
	key_handler = new CMenuScenceKeyHandler(this);
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

//
#define OBJECT_TYPE_MARIO	0

//Map objects
#define OBJECT_TYPE_BRICK	1
#define OBJECT_TYPE_SPIKE	21

//Enemy objects
#define OBJECT_TYPE_ENEMY1	2
#define OBJECT_TYPE_WORM	3
#define OBJECT_TYPE_SPIDER	10
#define OBJECT_TYPE_FLOATER	12
#define OBJECT_TYPE_DOME	13
#define OBJECT_TYPE_JUMPER	14
#define OBJECT_TYPE_CANNON	19
#define OBJECT_TYPE_EYEBALL	20
#define OBJECT_TYPE_TELEPORTER	33

//Main character objects
#define OBJECT_TYPE_MAIN_CHARACTER	9
#define OBJECT_TYPE_WHEEL_LEFT	4
#define OBJECT_TYPE_WHEEL_RIGHT	5
#define OBJECT_TYPE_WHEEL_MIDDLE	6
#define OBJECT_TYPE_CABIN	7
#define OBJECT_TYPE_BARREL	8
#define OBJECT_TYPE_HUMAN	11

#define OBJECT_TYPE_POWERHUB	500

#define OBJECT_TYPE_PORTAL	50

#define MAX_SCENE_LINE 1024


void CMenuScence::_ParseSection_TEXTURES(string line)
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

void CMenuScence::_ParseSection_SPRITES(string line)
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

void CMenuScence::_ParseSection_ANIMATIONS(string line)
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

void CMenuScence::_ParseSection_ANIMATION_SETS(string line)
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
void CMenuScence::_ParseSection_OBJECTS(string line)
{
	vector<string> tokens = split(line);

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	if (tokens.size() < 3) return; // skip invalid lines - an object set must have at least id, x, y

}



void CMenuScence::Load(int _alive, int _power)
{
	DebugOut(L"[INFO] Start loading scene resources from : %s \n", sceneFilePath);
	alives = _alive;
	power = _power;
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
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }

		//
		// data section
		//
		switch (section)
		{
		case SCENE_SECTION_TEXTURES: _ParseSection_TEXTURES(line); break;
		case SCENE_SECTION_SPRITES: _ParseSection_SPRITES(line); break;
		case SCENE_SECTION_ANIMATIONS: _ParseSection_ANIMATIONS(line); break;
		case SCENE_SECTION_ANIMATION_SETS: _ParseSection_ANIMATION_SETS(line); break;
		case SCENE_SECTION_OBJECTS: _ParseSection_OBJECTS(line); break;
		}
	}

	f.close();

	DebugOut(L"[INFO] Done loading scene resources %s\n", sceneFilePath);

}


void CMenuScence::Update(DWORD dt)
{
	CGame* game = CGame::GetInstance();
	if (isEnter)
	{
		if (isCancelButton)
		{
			game->SwitchScene(START_SCENCE_ID, 2, 8);
		}
		else if (isContinueButton)
		{
			if (scence_id != -1)
				game->SwitchScene(scence_id, 2, 8);
		}
	}
}

void CMenuScence::Render()
{
	CGame* game = CGame::GetInstance();
	if (alives >= 0)
	{
		CAnimationSets::GetInstance()->Get(0)->at(0)->isIntroEndScence = true;
		CAnimationSets::GetInstance()->Get(0)->at(0)->Render(80, 50);
		int ani = -1;
		switch (alives)
		{
		case 0:
			ani = 3;
			break;
		case 1:
			ani = 1;
			break;
		case 2:
			ani = 2;
			break;
		default:
			ani = 2;
			break;
		}
		CAnimationSets::GetInstance()->Get(0)->at(ani)->isIntroEndScence = true;
		CAnimationSets::GetInstance()->Get(0)->at(ani)->Render(180, 70);
		if (CAnimationSets::GetInstance()->Get(0)->at(0)->isFinish)
		{
			if (scence_id == -1)
				game->SwitchScene(START_SCENCE_ID, alives, power);
			else
				game->SwitchScene(scence_id, alives, power);
		}
	}
	else
	{
		CAnimationSets::GetInstance()->Get(0)->at(4)->isIntroEndScence = true;
		CAnimationSets::GetInstance()->Get(0)->at(4)->Render(80, 50);
		CAnimationSets::GetInstance()->Get(0)->at(5)->isIntroEndScence = true;
		if(isContinueButton)
			CAnimationSets::GetInstance()->Get(0)->at(5)->Render(60, 60);
		else
			CAnimationSets::GetInstance()->Get(0)->at(5)->Render(60, 80);
	}
}


/*
	Unload current scene
*/
void CMenuScence::Unload()
{
	if (CAnimationSets::GetInstance()->Get(0))
		CAnimationSets::GetInstance()->Get(0)->at(0)->isIntroEndScence = false;
	DebugOut(L"[INFO] Scene %s unloaded! \n", sceneFilePath);
	isEnter = false;
	isContinueButton = true;
	isCancelButton = false;
}

void CMenuScenceKeyHandler::OnKeyDown(int KeyCode)
{

}

void CMenuScenceKeyHandler::OnKeyUp(int KeyCode)
{
	CGame* game = CGame::GetInstance();
	switch (KeyCode)
	{
	case DIK_A:
		break;
	case DIK_UP:
		if (!isContinueButton)
		{
			isContinueButton = true;
			isCancelButton = false;
		}
		break;
	case DIK_DOWN:
		if (!isCancelButton)
		{
			isCancelButton = true;
			isContinueButton = false;
		}
		break;
	case DIK_SPACE:
		isEnter = true;
		break;
	}
}

void CMenuScenceKeyHandler::KeyState(BYTE* states)
{

}

