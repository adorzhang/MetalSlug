#include <iostream>
#include <fstream>

#include "PlayScence.h"
#include "Utils.h"
#include "Textures.h"
#include "Sprites.h"
#include "Portal.h"


//#include "Grid.h"
#include "GunHub.h"
#include "PowerHub.h"
#include "Sound.h"
#include "MenuScence.h"
using namespace std;

CPlayScene::CPlayScene(int id, LPCWSTR filePath) :
	CScene(id, filePath)
{
	key_handler = new CPlayScenceKeyHandler(this);
	cam_y = CGame::GetInstance()->GetScreenHeight();
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
#define OBJECT_TYPE_BRICK	4
#define OBJECT_TYPE_LAVA	30
#define OBJECT_TYPE_SPIKE	21
#define OBJECT_TYPE_STAIR	22
#define OBJECT_TYPE_ARROW	31

//Enemy objects
#define OBJECT_TYPE_ENEMY1	2
#define OBJECT_TYPE_CHOWMEIN	3
#define OBJECT_TYPE_SPIDER	10
#define OBJECT_TYPE_FLOATER	12
#define OBJECT_TYPE_DOME	13
#define OBJECT_TYPE_JUMPER	14

#define OBJECT_TYPE_INSECT	15

#define OBJECT_TYPE_LOCUST		16
#define OBJECT_TYPE_SKULL	17
#define OBJECT_TYPE_CANNON	19
#define OBJECT_TYPE_EYEBALL	20
#define OBJECT_TYPE_TELEPORTER 33
#define OBJECT_TYPE_ITEM 34
#define OBJECT_TYPE_MINE 40
#define OBJECT_TYPE_EGG 41

//Main character objects
#define OBJECT_TYPE_MAIN_CHARACTER	9

#define OBJECT_TYPE_BARREL	7

#define OBJECT_TYPE_HUMAN	11

#define OBJECT_TYPE_POWERHUB	500

#define OBJECT_TYPE_PORTAL	50

#define OBJECT_TYPE_SPECIAL_PORTAL	51

#define MAX_SCENE_LINE 1024

#define OBJECT_TYPE_LEGS 42


void CPlayScene::_ParseSection_TEXTURES(string line)
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

void CPlayScene::_ParseSection_SPRITES(string line)
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

void CPlayScene::_ParseSection_ANIMATIONS(string line)
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

void CPlayScene::_ParseSection_ANIMATION_SETS(string line)
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
void CPlayScene::_ParseSection_OBJECTS(string line)
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
		//if (type_scence == OVER_WORLD)
			//player->Is_Human = true;
		DebugOut(L"[INFO] Player object created!\n");
		return;
		break;
	case OBJECT_TYPE_CHOWMEIN:
	{
		int item = 0;
		if (tokens.size() > 5)
			item = atoi(tokens[5].c_str());
		obj = new CChowmein(item);
		LPANIMATION_SET ani_set = animation_sets->Get(200);
		obj->SetAnimationItemSet(ani_set);
	}
	break;
	case OBJECT_TYPE_FLOATER:
	{
		int item = 0;
		if (tokens.size() > 5)
			item = atoi(tokens[5].c_str());
		obj = new CFloater(item);
		LPANIMATION_SET ani_set = animation_sets->Get(200);
		obj->SetAnimationItemSet(ani_set);
	}
	break;
	case OBJECT_TYPE_MINE:
	{
		int item = 0;
		if (tokens.size() > 5)
			item = atoi(tokens[5].c_str());
		obj = new CMine(item);
		LPANIMATION_SET ani_set = animation_sets->Get(200);
		obj->SetAnimationItemSet(ani_set);
	}
	break;
	case OBJECT_TYPE_CANNON:
	{
		int item = 0;
		if (tokens.size() > 5)
			item = atoi(tokens[5].c_str());
		obj = new CCannon(item);
		LPANIMATION_SET ani_set = animation_sets->Get(200);
		obj->SetAnimationItemSet(ani_set);
	}
	break;
	case OBJECT_TYPE_EYEBALL:
	{
		int item = 0;
		if (tokens.size() > 5)
			item = atoi(tokens[5].c_str());
		obj = new CEyeball(item);
		LPANIMATION_SET ani_set = animation_sets->Get(200);
		obj->SetAnimationItemSet(ani_set);
	}
	break;
	case OBJECT_TYPE_TELEPORTER:
	{
		int item = 0;
		if (tokens.size() > 5)
			item = atoi(tokens[5].c_str());
		obj = new CTeleporter(item);
		LPANIMATION_SET ani_set = animation_sets->Get(200);
		obj->SetAnimationItemSet(ani_set);
	}
	break;
	case OBJECT_TYPE_DOME:
	{
		int item = 0;
		if (tokens.size() > 5)
			item = atoi(tokens[5].c_str());
		obj = new CDome(item);
		LPANIMATION_SET ani_set = animation_sets->Get(200);
		obj->SetAnimationItemSet(ani_set);
	}
	break;
	case OBJECT_TYPE_JUMPER:
	{
		int item = 0;
		if (tokens.size() > 5)
			item = atoi(tokens[5].c_str());
		obj = new CJumper(item);
		LPANIMATION_SET ani_set = animation_sets->Get(200);
		obj->SetAnimationItemSet(ani_set);
	}
	break;
	case OBJECT_TYPE_INSECT:
	{
		int item = 0;
		if (tokens.size() > 5)
			item = atoi(tokens[5].c_str());
		obj = new CInsect(item);
		LPANIMATION_SET ani_set = animation_sets->Get(200);
		obj->SetAnimationItemSet(ani_set);
	}
	break;
	case OBJECT_TYPE_SKULL:
	{
		int item = 0;
		if (tokens.size() > 5)
			item = atoi(tokens[5].c_str());
		obj = new CSkull(item);
		LPANIMATION_SET ani_set = animation_sets->Get(200);
		obj->SetAnimationItemSet(ani_set);
	}
	break;
	case OBJECT_TYPE_LOCUST:
	{
		int item = 0;
		int type = 0;
		if (tokens.size() > 6)
		{
			type = atoi(tokens[5].c_str());
			item = atoi(tokens[6].c_str());
		}

		obj = new CLocust(item, type);
		LPANIMATION_SET ani_set = animation_sets->Get(200);
		obj->SetAnimationItemSet(ani_set);
	}
	break;
	case OBJECT_TYPE_SPIKE:
	{
		float r = atof(tokens[5].c_str());
		float b = atof(tokens[6].c_str());
		obj = new CSpike(x, y, r, b);
		break;
	}
	case OBJECT_TYPE_ARROW:
	{
		float r = atof(tokens[5].c_str());
		float b = atof(tokens[6].c_str());
		obj = new CArrow(x, y, r, b);
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
	case OBJECT_TYPE_BRICK:
	{
		float r = atof(tokens[5].c_str());
		float b = atof(tokens[6].c_str());
		if (tokens.size() > 7)
		{
			int type = atoi(tokens[7].c_str());
			obj = new CBrick(x, y, r, b, type);

		}
		else
			obj = new CBrick(x, y, r, b);
		break;
	}
	case OBJECT_TYPE_EGG:
	{
		if (tokens.size() > 5)
		{
			int item = atoi(tokens[5].c_str());
			obj = new CEgg(item);
			LPANIMATION_SET ani_set = animation_sets->Get(200);
			obj->SetAnimationItemSet(ani_set);
		}
		else
			obj = new CEgg(0);
		break;
	}
	case OBJECT_TYPE_LAVA:
	{
		float r = atof(tokens[5].c_str());
		float b = atof(tokens[6].c_str());
		obj = new CLava(x, y, r, b);
		break;
	}
	case OBJECT_TYPE_STAIR:
	{
		float r = atof(tokens[5].c_str());
		float b = atof(tokens[6].c_str());
		obj = new CStair(x, y, r, b);
		break;
	}
	case OBJECT_TYPE_LEGS: {
		obj = new CLegs();
		float _x, _y;
		obj->SetAnimationSet(animation_sets->Get(ani_set_id));
		obj->SetID(object_id);
		if (player != NULL)
		{

			DebugOut(L"[INFO] Player object has been Created Already!\n");
			player->AddComponentObject(obj);
			player->GetPosition(_x, _y);
			obj->SetPosition(_x, _y);
			return;
		}
		break;
	}
	case OBJECT_TYPE_BARREL:
	{
		obj = new CBarrel();
		float _x, _y;
		obj->SetID(object_id);
		obj->SetAnimationSet(animation_sets->Get(ani_set_id));
		if (player != NULL)
		{
			DebugOut(L"[INFO] Player object has been Created Already!\n");
			player->AddComponentObject(obj);
			player->GetPosition(_x, _y);
			obj->SetPosition(_x, _y);
		}
		return;
		break;
	}
	case OBJECT_TYPE_ITEM:
	{
		int type = 0;
		if (tokens.size() > 5)
			type = atoi(tokens[5].c_str());
		obj = new CItem(type);
	}
	break;
	case OBJECT_TYPE_SPECIAL_PORTAL:
	{
		float r = atof(tokens[5].c_str());
		float b = atof(tokens[6].c_str());
		int scene_id = atoi(tokens[7].c_str());
		int next_portal_id = atoi(tokens[8].c_str());
		obj = new CSpecialPortal(x, y, r, b, scene_id, next_portal_id);
		if (CGame::GetInstance()->GetIsPreMap())
		{
			if (CGame::GetInstance()->GetNextPortalId() == object_id)
			{
				if (player != NULL)
				{
					float old_x_player = CGame::GetInstance()->GetParamXPlayer();
					float old_y_player = CGame::GetInstance()->GetParamYPlayer();
					//player->Is_Human = true;
					//player->SetState(MAIN_CHARACTER_STATE_OPEN_CABIN);
					player->SetPosition(old_x_player, old_y_player);

				}
			}

		}
	}
	break;
	case OBJECT_TYPE_PORTAL:
	{
		float r = atof(tokens[5].c_str());
		float b = atof(tokens[6].c_str());
		int scene_id = atoi(tokens[7].c_str());
		int type = atoi(tokens[8].c_str());
		int next_portal_id = atoi(tokens[9].c_str());
		obj = new CPortal(x, y, r, b, scene_id, type, next_portal_id);
		if (CGame::GetInstance()->GetIsPreMap())
		{
			if (player != NULL)
			{
				if (CGame::GetInstance()->GetNextPortalId() == object_id)
				{
					player->SetPosition((x - MAIN_CHARACTER_BBOX_WIDTH) - 2, y);
				}

			}
		}
		else if (CGame::GetInstance()->GetIsNextMap())
		{
			if (player != NULL)
			{
				if (CGame::GetInstance()->GetNextPortalId() == object_id)
				{
					player->SetPosition(x + (r - x) + 2, y);
				}
			}
		}
		else if (CGame::GetInstance()->GetIsUpMap())
		{
			if (player != NULL)
			{
				if (CGame::GetInstance()->GetNextPortalId() == object_id)
				{
					player->SetPosition(x, y + MAIN_CHARACTER_BBOX_HEIGHT + 12);
				}
			}
		}
		else if (CGame::GetInstance()->GetIsDownMap())
		{
			if (player != NULL)
			{
				if (CGame::GetInstance()->GetNextPortalId() == object_id)
				{
					player->SetPosition(x, y - (y - b) - 6);
				}
			}
		}
	}
	break;
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

/*
	Parse a line in section [MAP]
*/
void CPlayScene::_ParseSection_MAP(string line)
{
	vector<string> tokens = split(line);

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	if (tokens.size() < 1) return; // skip invalid lines - an map must have at least path

	wstring path = ToWSTR(tokens[0]);

	tiledMap = new CTiledMap();
	tiledMap->LoadMap(path.c_str());

	vector<string> fg = tiledMap->getForeground();
	vector<string> bg = tiledMap->getBackground();
	if (fg.size() > 0) {
		float x = atof(fg[2].c_str());
		float y = atof(fg[3].c_str());
		int ani_set_id = atoi(fg[4].c_str());
		CAnimationSets* animation_sets = CAnimationSets::GetInstance();
		foreground = new CForeground(x, y);
		foreground->SetPosition(x, y);
		foreground->SetAnimationSet(animation_sets->Get(ani_set_id));
	}
	if (bg.size() > 0) {
		float x = atof(bg[2].c_str());
		float y = atof(bg[3].c_str());
		int ani_set_id = atoi(bg[4].c_str());
		CAnimationSets* animation_sets = CAnimationSets::GetInstance();
		background = new CBackground(x, y);
		background->SetPosition(x, y);
		background->SetAnimationSet(animation_sets->Get(ani_set_id));
	}


	
	CTiledMapSets::GetInstance()->Add(id, tiledMap);




}

void CPlayScene::_ParseSection_GRID(string line)
{
	// check flag to init grid
	if (initGridFlag)
	{
		int width, height;
		//CTiledMap::GetInstance()->GetMapWidth(width);
		//CTiledMap::GetInstance()->GetMapHeight(height);
		CTiledMapSets::GetInstance()->Get(id)->GetMapWidth(width);
		CTiledMapSets::GetInstance()->Get(id)->GetMapHeight(height);
		int numCol = width / CELL_WIDTH;
		int numRow = height / CELL_HEIGHT;

		(height % CELL_HEIGHT == 0) ? numRow = numRow : numRow = numRow += 1;
		(width % CELL_HEIGHT == 0) ? numCol = numCol : numCol = numCol += 1;

		CGrid::GetInstance()->SetCol(numCol);
		CGrid::GetInstance()->SetRow(numRow);

		CGrid::GetInstance()->Init();

		initGridFlag = false;
	}

	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines
	else
	{
		int cellX = atoi(tokens[0].c_str());
		int cellY = atoi(tokens[1].c_str());

		for (int i = 2; i < tokens.size(); i++)
		{
			int object_id = atoi(tokens[i].c_str());
			for (int j = 0; j < objects.size(); j++)
			{
				LPGAMEOBJECT object = objects.at(j);
				if (object->GetID() == object_id)
				{
					CGrid::GetInstance()->Add(object, cellX, cellY);
				}
			}
		}
	}
}

void CPlayScene::Load(int _alive, int _power)
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
		if (line == "[GRID]") {
			section = SCENE_SECTION_GRID; continue;
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
			//bổ sung
		case SCENE_SECTION_MAP: _ParseSection_MAP(line); break;
		case SCENE_SECTION_GRID:
		{
			_ParseSection_GRID(line);
			break;
		}
		}
	}

	f.close();
	//Texture for bounding box
	CTextures::GetInstance()->Add(ID_TEX_BBOX, L"textures\\bbox.png", D3DCOLOR_XRGB(255, 255, 255));

	DebugOut(L"[INFO] Done loading scene resources %s\n", sceneFilePath);

	if (player != NULL)
	{
		player->SetPower(_power);
		player->SetAlive(_alive);
	}

	gameCamera = Camera::GetInstance();
}

//float cam_y = CGame::GetInstance()->GetScreenHeight();
void CPlayScene::Update(DWORD dt)
{
	// We know that Mario is the first object in the list hence we won't add him into the colliable object list
	// TO-DO: This is a "dirty" way, need a more organized way 

	// get objects from grid
	CGame* game = CGame::GetInstance();
	if (!isMenuScenceDisplayed && id == START_SCENCE_ID)
	{
		isMenuScenceDisplayed = true;
		CMenuScence* menu_scence = dynamic_cast<CMenuScence*>(CGame::GetInstance()->GetScene(MENU_SCENCE_ID));
		if (menu_scence)
			menu_scence->SetScenceId(id);
		game->SwitchScene(MENU_SCENCE_ID, 2, 8);
	}
	else
	{
		if (background) background->Update(dt);
		if (foreground) foreground->Update(dt);

		map_objects = CGrid::GetInstance()->GetList();
		vector<LPGAMEOBJECT> coObjects;
		if (map_objects.size() > 0)
		{
			for (auto object : map_objects)
				coObjects.push_back(object.second);
			//{
				for (auto object : map_objects)
					object.second->Update(dt, &coObjects);
			//}
		}
		if (player == NULL) return;
		else
		{
			player->Update(dt, &coObjects);

		}
		// skip the rest if scene was already unloaded (Mario::Update might trigger PlayScene::Unload)

		//Thực hiện chuyển sang scene tiếp theo
		if (game->GetIsNextMap() == true)
		{
			if (type_scence == OVER_WORLD)
			{
				// switch scene
				game->SwitchScene(game->GetSceneId(), player->GetAlive(), player->GetPower());
				game->SetIsNextMap(false);
			}
			else
			{
				game->SetRenderingNextMap(true);
				//Lấy scene id của scene tiếp theo từ game object
				id_next_map = game->GetSceneId();
				game->SetScenceIDRenderingNextMap(id_next_map);
				//Get scene kế tiếp thông qua scene_id
				LPSCENE s = game->GetScene(id_next_map);
				//Nếu scene tiếp theo tồn tại và chưa load tiled map của scene tiếp theo
				if (s && initNextMap)
				{
					// Lấy tiled map tiếp theo
					s->GetNextMap();
					// Nếu tiled map tiếp theo tồn tại
					if (s->GetMap() != NULL)
					{
						//Thêm vào đối tượng CTiledMapSets
						CTiledMapSets::GetInstance()->Add(id_next_map, s->GetMap());
						//Chuyển cờ đánh dấu đã load được tiled map tiếp theo
						initNextMap = false;
					}

				}
				float player_x, player_y;
				player->GetPosition(player_x, player_y);
				//Không xét va chạm và render player lên màn hình
				player->SetState(MAIN_CHARACTER_STATE_NONE_COLLISION);
				//Cho camera di chuyển theo trục x
				player->SetSpeed(0.4, 0);
				//Lấy width, height của map hiện tại
				CMap* map = CTiledMapSets::GetInstance()->Get(id);
				int widthMap, heightMap;
				map->GetMapWidth(widthMap);
				map->GetMapHeight(heightMap);
				//Lấy width, height của map tiếp theo
				map = CTiledMapSets::GetInstance()->Get(id_next_map);
				int widthNextMap, heightNextMap;
				map->GetMapWidth(widthNextMap);
				map->GetMapHeight(heightNextMap);
				player->GetPosition(player_x, player_y);
				//Sau hiệu ứng di chuyển camera sang màn thì tiến hành chuyển màn
				if (player_x >= widthMap + (widthNextMap / 3))
				{
					// switch scene
					game->SwitchScene(game->GetSceneId(), player->GetAlive(), player->GetPower());
					game->SetIsNextMap(false);
				}
			}
		}
		//Thực hiện chuyển về scene trước
		else if (game->GetIsPreMap() == true)
		{
			if (CGame::GetInstance()->GetParamEnteringOverWorld())
			{
				if (player)
				{
					float player_x, player_y;
					player->GetPosition(player_x, player_y);
					CGame::GetInstance()->SetParamPlayer(player_x, player_y);
				}
				CGame::GetInstance()->SetParamEnteringOverWorld(false);
			}
			// switch scene
			game->SwitchScene(game->GetSceneId(), player->GetAlive(), player->GetPower());
			game->SetIsPreMap(false);

		}
		else if (game->GetIsUpMap() == true)
		{
			if (type_scence == OVER_WORLD)
			{
				// switch scene
				game->SwitchScene(game->GetSceneId(), player->GetAlive(), player->GetPower());
				game->SetIsUpMap(false);
			}
		}
		else if (game->GetIsDownMap() == true)
		{
			if (type_scence == OVER_WORLD)
			{
				// switch scene
				game->SwitchScene(game->GetSceneId(), player->GetAlive(), player->GetPower());
				game->SetIsDownMap(false);
			}
		}
		else
			game->SetRenderingNextMap(false);

		CMap* map = CTiledMapSets::GetInstance()->Get(id);
		int widthMap, heightMap;
		map->GetMapWidth(widthMap);
		map->GetMapHeight(heightMap);
		// Update camera to follow main character
		//Xử lý camera theo trục x
		float cx = 0, cy = 0;
		if (player)
		{
			player->GetPosition(cx, cy);

			if (cx <= (float)game->GetScreenWidth() / 2)
			{
				cx = 0;
			}
			else if (widthMap - cx <= (float)game->GetScreenWidth() / 2)
			{
				if (game->GetIsNextMap())//nếu player va chạm portal
					cx -= (float)game->GetScreenWidth() / 2;
				else
					cx = widthMap - game->GetScreenWidth();
			}
			else
			{
				cx -= (float)game->GetScreenWidth() / 2;
				
			}
		}
		//Xử lý camera theo trục y
		cy = game->GetScreenHeight();

		if (player)
		{
			float player_x, player_y;
			player_x = 0;
			player_y = 0;

			player->GetPosition(player_x, player_y);

			if (player_y > (float)game->GetScreenHeight() / 2 && player_y < heightMap - (float)(game->GetScreenHeight() / 2) - 1 && id != 1 && id != 5 && id != 3 && id != 22 && id != 23)
			{
				cam_y = player_y + (float)game->GetScreenHeight() / 2;
			}

		}
		CGame::GetInstance()->SetCamPos(cx, cam_y);
		//Vẽ Hub objects
		/*for (int i = 0; i < hub_objects.size(); i++)
			hub_objects[i]->Update(dt);*/
		Sound::getInstance()->Play(SOUND_ID_AREA_2);

		
	}
}

void CPlayScene::Render()
{
	//LPDIRECT3DTEXTURE9 maptextures = CTextures::GetInstance()->Get(67);
	//background
	//CGame::GetInstance()->Draw(-5, 130, maptextures, 0, 0, 5169, 273); //TODO: cal position
	//forceground
	//CGame::GetInstance()->Draw(-5, 185, maptextures, 0, 1196, 5169, 1418); //TODO: cal position

	if (background != NULL) background->Render();
	if (foreground != NULL) foreground->Render();

	//Vẽ tiled map của scene hiện tại
	CTiledMapSets::GetInstance()->Get(id)->Render();
	//Tạo hiệu ứng vẽ tiled map của scene tiếp theo nếu thỏa điều kiện
	if (id_next_map != -1 && CTiledMapSets::GetInstance()->Get(id_next_map) && CGame::GetInstance()->GetRenderingNextMap())
	{
		CMap* map = CTiledMapSets::GetInstance()->Get(id);
		int widthMap, heightMap;
		map->GetMapWidth(widthMap);
		map->GetMapHeight(heightMap);

		map = CTiledMapSets::GetInstance()->Get(id_next_map);
		int widthNextMap, heightNextMap;
		map->GetMapWidth(widthNextMap);
		map->GetMapHeight(heightNextMap);
		CTiledMapSets::GetInstance()->Get(id_next_map)->Render(widthMap, heightMap <= heightNextMap ? 0 : heightMap - heightNextMap);
	}

	//Vẽ tất cả các object hiện tại nếu thỏa điều kiện
	if (player->GetState() != MAIN_CHARACTER_STATE_NONE_COLLISION)
	{
		for (auto object : map_objects)
			object.second->Render();
		//Vẽ player object
		if (type_scence != OVER_WORLD)
		{
			player->Render();
		}

		if (player->GetPower() < 0 && player->GetState() == MAIN_CHARACTER_STATE_DIE)
		{
			int lives = player->GetAlive();
			if (lives >= 0)
			{
				lives -= 1;
				player->SetAlive(lives);
				ReLoad();
			}
		}
		//Vẽ Hub objects
		/*for (int i = 0; i < hub_objects.size(); i++)
			hub_objects[i]->Render();*/
	}
}

CMap* CPlayScene::GetMap()
{
	return tiledMap;
}

void CPlayScene::GetNextMap()
{
	sprites_next_map = new CSprites();
	ifstream f;
	f.open(sceneFilePath);

	// current resource section flag
	int section = SCENE_SECTION_UNKNOWN;
	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	
		if (line == "[MAP]") {
			section = SCENE_SECTION_MAP; continue;
		}
		if (line == "[SPRITES]") {
			section = SCENE_SECTION_SPRITES; continue;
		}
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }

		//
		// data section
		//
		switch (section)
		{
		case SCENE_SECTION_MAP:
		{
			vector<string> tokens = split(line);
			if (tokens.size() < 1) return; // skip invalid lines - an map must have at least path
			wstring path = ToWSTR(tokens[0]);
			tiledMap = new CTiledMap();
			tiledMap->LoadMap(path.c_str());
			break;
		}
		case SCENE_SECTION_SPRITES:
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
			sprites_next_map->Add(ID, l, t, r, b, tex);
			break;
		}
		}
	}

	f.close();

	DebugOut(L"[INFO] Done loading next map resources %s\n", sceneFilePath);

}
/*
	Unload current scene
*/
void CPlayScene::Unload()
{
	cam_y = CGame::GetInstance()->GetScreenHeight();
	CGame::GetInstance()->SetRenderingNextMap(false);
	sprites_next_map = NULL;
	objects.clear();
	map_objects.clear();
	player = NULL;
	//player_human = NULL;
	isRenderNextMap = false;
	isRenderPreMap = false;
	initNextMap = true;
	initGridFlag = true;
	hub_objects.clear();
	CGrid::GetInstance()->Unload();
	DebugOut(L"[INFO] Scene %s unloaded! \n", sceneFilePath);
}
void CPlayScene::ReLoad()
{
	Sound::getInstance()->StopAll();
	if (player)
	{
		CMenuScence* menu_scence = dynamic_cast<CMenuScence*>(CGame::GetInstance()->GetScene(MENU_SCENCE_ID));
		if (menu_scence)
			menu_scence->SetScenceId(id);
		//if(player->GetAlive()>=0)
		CGame::GetInstance()->SwitchScene(MENU_SCENCE_ID, player->GetAlive(), 8);
		//	else
				//CGame::GetInstance()->SwitchScene(MENU_SCENCE_ID, 2, 8);
	}
}
void CPlayScenceKeyHandler::OnKeyDown(int KeyCode)
{
	//DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);
	CMainCharacter* player = ((CPlayScene*)scence)->GetPlayer();
	float player_x, player_y;
	player->GetPosition(player_x, player_y);
	switch (KeyCode)
	{
	case DIK_UP:

		player->SetState(MAIN_CHARACTER_STATE_UP_BARREL);
		break;
	case DIK_SPACE:
		player->SetState(MAIN_CHARACTER_STATE_JUMP);
		break;
	case DIK_A:
		player->Reset();
		break;
	case DIK_Z:
		player->SetState(MAIN_CHARACTER_STATE_BARREL_FIRE);
		Sound::getInstance()->PlayNew(SOUND_ID_BULLET_FIRE);
		break;
	case DIK_X:
		player->SetState(MAIN_CHARACTER_STATE_FIRE_ROCKET);
	case DIK_M:

		break;
	}
}
void CPlayScenceKeyHandler::OnKeyUp(int KeyCode)
{
	switch (KeyCode)
	{
	case DIK_SPACE:

		break;
	case DIK_A:

		break;
	case DIK_Z:

		break;
	case DIK_X:

		break;
	case DIK_M:

		break;
	}
}

void CPlayScenceKeyHandler::KeyState(BYTE* states)
{
	CGame* game = CGame::GetInstance();
	CMainCharacter* player = ((CPlayScene*)scence)->GetPlayer();
	//// disable control key when Mario die 
	if (player->GetState() == MAIN_CHARACTER_STATE_DIE) return;
	if (player->GetState() == MAIN_CHARACTER_STATE_NONE_COLLISION) return;
	if (CGame::GetInstance()->GetCurrentScenceID() == SCENCE_ID_OVERWORLD_END) return;

	else if (game->IsKeyDown(DIK_DOWN))
	{
		player->SetState(MAIN_CHARACTER_STATE_DOWN_BARREL);
	}
	else if (game->IsKeyDown(DIK_RIGHT))
	{
		player->SetState(MAIN_CHARACTER_STATE_RUN_RIGHT);
	}

	else if (game->IsKeyDown(DIK_LEFT))
	{
		player->SetState(MAIN_CHARACTER_STATE_RUN_LEFT);
	}
	else
	{
		player->SetState(MAIN_CHARACTER_STATE_IDLE);
	}
}
