#include <iostream>
#include <fstream>

#include "PlayScene.h"
#include "Utils.h"
#include "Animations.h"

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/filereadstream.h"

#pragma region Include Game Object
#include "Sophia.h"
#include "Jason.h"
#include "BigJason.h"

#include "Portal.h"
#include "MiniPortal.h"
#include "BossTrigger.h"

#include "Brick.h"
#include "Thorn.h"
#include "BreakableBrick.h"

#include "Interrupt.h"
#include "Neoworm.h"
#include "Ballbot.h"
#include "Stuka.h"
#include "Eyelet.h"
#include "BallCarry.h"
#include "Drap.h"
#include "GX680.h"
#include "GX680S.h"
#include "LaserGuard.h"
#include "BossZ88.h"

#include "HealthBar.h"
#pragma endregion

CPlayScene::CPlayScene(int id, LPCWSTR filePath) :
	CScene(id, filePath)
{
	state = PlaySceneState::FreePlaying;
	key_handler = new CPlayScenceKeyHandler(this);
}

CPlayScene::~CPlayScene()
{
}

/*
	Load scene resources from scene file (textures, sprites, animations and objects)
	See scene1.txt, scene2.txt for detail format specification
*/

#define SCENE_SECTION_UNKNOWN			-1
#define SCENE_SECTION_TEXTURES			1
#define SCENE_SECTION_SPRITES			2
#define SCENE_SECTION_ANIMATIONS		3
#define SCENE_SECTION_MAP				4

#define MAX_SCENE_LINE 1024

void CPlayScene::Load()
{
	DebugOut(L"[INFO] Start loading scene resources from : %s \n", sceneFilePath);

	auto game = CGame::GetInstance();

	// Init Camera
	auto mainCam = game->GetComponent<CCamera>();
	mainCam->SetBoundingBoxSize(Vector2(game->GetScreenWidth(), game->GetScreenHeight()));

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
		if (line == "[SPRITES]") { section = SCENE_SECTION_SPRITES; continue; }
		if (line == "[ANIMATIONS]") { section = SCENE_SECTION_ANIMATIONS; continue; }
		if (line == "[MAP]") { section = SCENE_SECTION_MAP; continue; }
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }

		//
		// data section
		//
		switch (section)
		{
		case SCENE_SECTION_TEXTURES: _ParseSection_TEXTURES(line); break;
		case SCENE_SECTION_SPRITES: _ParseSection_SPRITES(line); break;
		case SCENE_SECTION_ANIMATIONS: _ParseSection_ANIMATIONS(line); break;
		case SCENE_SECTION_MAP: _ParseSection_MAP(line); break;
		}
	}

	f.close();

	DebugOut(L"[INFO] Done loading scene resources %s\n", sceneFilePath);
}

void CPlayScene::_ParseSection_TEXTURES(std::string line)
{
	if (state == PlaySceneState::Switching) return;

	vector<string> tokens = split(line);

	if (tokens.size() < 5) return; // skip invalid lines

	std::string texID = tokens[0].c_str();
	wstring path = ToWSTR(tokens[1]);
	int R = atoi(tokens[2].c_str());
	int G = atoi(tokens[3].c_str());
	int B = atoi(tokens[4].c_str());

	CGame::GetInstance()->GetComponent<CTextures>()->Add(texID, path.c_str(), D3DCOLOR_XRGB(R, G, B));
}

void CPlayScene::_ParseSection_SPRITES(std::string line)
{
	if (state == PlaySceneState::Switching) return;

	vector<string> tokens = split(line);

	if (tokens.size() < 6) return; // skip invalid lines

	std::string ID = tokens[0].c_str();
	int l = atoi(tokens[1].c_str());
	int t = atoi(tokens[2].c_str());
	int r = atoi(tokens[3].c_str());
	int b = atoi(tokens[4].c_str());
	std::string texID = tokens[5].c_str();

	LPDIRECT3DTEXTURE9 tex = CGame::GetInstance()->GetComponent<CTextures>()->Get(texID);
	if (tex == NULL)
	{
		DebugOut(L"[ERROR] Texture ID %d not found!\n", texID);
		return;
	}

	CGame::GetInstance()->GetComponent<CSprites>()->Add(ID, l, t, r, b, tex);
}

void CPlayScene::_ParseSection_ANIMATIONS(std::string line)
{
	if (state == PlaySceneState::Switching) return;

	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	LPANIMATION ani = new CAnimation();

	std::string ani_id = tokens[0].c_str();

	for (int i = 1; i < tokens.size(); i += 2)	// why i+=2 ?  sprite_id | frame_time  
	{
		std::string sprite_id = tokens[i].c_str();
		int frame_time = atoi(tokens[i + 1].c_str());
		ani->Add(sprite_id, frame_time);
	}

	CGame::GetInstance()->GetComponent<CAnimations>()->Add(ani_id, ani);
}

void CPlayScene::_ParseSection_MAP(std::string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 1 || tokens[0] == "") return;

	auto game = CGame::GetInstance();

	string filePath = tokens[0];

	FILE* fp;
	errno_t err = fopen_s(&fp, filePath.c_str(), "r");

	char readBuffer[65536];
	rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));

	rapidjson::Document d;
	d.ParseStream(is);

	int tileWidth = d["tilewidth"].GetInt();
	int tileHeight = d["tileheight"].GetInt();

	int mapWidth = d["width"].GetInt();			// Calculate by tile
	int mapHeight = d["height"].GetInt();

	// Set boundary of camera
	auto mainCam = game->GetComponent<CCamera>();

	Rect boundary;
	boundary.left = 0;
	boundary.top = mapHeight * tileHeight;
	boundary.right = mapWidth * tileWidth;
	boundary.bottom = 0;
	mainCam->GetBoundary(boundary);

	// Init Quadtree
	m_mapWidth = mapWidth * tileWidth;
	m_mapHeight = mapHeight * tileHeight;
	quadtree = new CQuadtree(0, Rect(0, m_mapHeight, m_mapWidth, 0));
	quadtree->Reset(m_mapWidth, m_mapHeight);

	// Set map texture
	auto properties = d["properties"].GetArray();

	for (auto& prop : properties)
	{
		if (strcmp(prop["name"].GetString(), "Background") == 0)
		{
			std::string image_path = prop["value"].GetString();
			std::string map_name = image_path;
			map_name.erase(map_name.end() - 4, map_name.end());
			map_name.erase(map_name.begin(), map_name.begin() + 5);

			std::string texID = "tex-" + map_name;
			auto textures = game->GetComponent<CTextures>();
			textures->Add(texID, ToWSTR(image_path).c_str(), D3DCOLOR_XRGB(0, 0, 0));
			auto texmap = textures->Get(texID);

			background = new CMapSprite(m_mapWidth, m_mapHeight, texmap);
			background->SetPosition(Vector2(m_mapWidth / 2, m_mapHeight / 2));
		}
		else if (strcmp(prop["name"].GetString(), "Foreground") == 0)
		{
			std::string image_path = prop["value"].GetString();
			std::string map_name = image_path;
			map_name.erase(map_name.end() - 4, map_name.end());
			map_name.erase(map_name.begin(), map_name.begin() + 5);

			std::string texID = "tex-" + map_name;
			auto textures = game->GetComponent<CTextures>();
			textures->Add(texID, ToWSTR(image_path).c_str(), D3DCOLOR_XRGB(0, 0, 0));
			auto texmap = textures->Get(texID);

			foreground = new CMapSprite(m_mapWidth, m_mapHeight, texmap);
			foreground->SetPosition(Vector2(m_mapWidth / 2, m_mapHeight / 2));
		}
		else if (strcmp(prop["name"].GetString(), "IsTopDown") == 0)
		{
			isTopDownView = prop["value"].GetBool();
		}
	}

	// Insert platform objects
	auto layers = d["layers"].GetArray();

	for (auto& layer : layers)
	{
		auto layer_type = layer["type"].GetString();
		auto visible = layer["visible"].GetBool();

		// Object Layer
		if (strcmp(layer_type, "objectgroup") == 0 && visible == true)
		{
			auto objects = layer["objects"].GetArray();

			for (auto& object : objects)
			{
				CGameObject* obj = nullptr;

				int x = object["x"].GetInt();
				int y = object["y"].GetInt();

				int width = object["width"].GetInt();
				int height = object["height"].GetInt();

				auto object_name = object["name"].GetString();

				if (strcmp(object_name, "jason") == 0) obj = new CJason;
				else if (strcmp(object_name, "sophia") == 0)
				{
					if (player != nullptr)
					{
						DebugOut(L"[ERROR] SOPHIA object was created before!\n");
						continue;
					}
					obj = new CSophia;
					SetPlayer(obj);

					DebugOut(L"[INFO] SOPHIA created!\n");
				}
				else if (strcmp(object_name, "bigjason") == 0)
				{
					if (player != nullptr)
					{
						DebugOut(L"[ERROR] BIG JASON object was created before!\n");
						continue;
					}
					obj = new CBigJason;
					SetPlayer(obj);

					DebugOut(L"[INFO] BIG JASON object created!\n");
				}
				else if (strcmp(object_name, "interrupt") == 0) obj = new CInterrupt;
				else if (strcmp(object_name, "neoworm") == 0) obj = new CNeoworm;
				else if (strcmp(object_name, "ballbot") == 0) obj = new CBallbot;
				else if (strcmp(object_name, "stuka") == 0) obj = new CStuka;
				else if (strcmp(object_name, "eyelet") == 0)
				{
					obj = new CEyelet;
					((CEyelet*)obj)->SetInitAxisY(m_mapHeight - y + height / 2);
				}
				else if (strcmp(object_name, "ballcarry") == 0) obj = new CBallCarry;
				else if (strcmp(object_name, "drap") == 0) obj = new CDrap;
				else if (strcmp(object_name, "gx680") == 0) obj = new CGX680;
				else if (strcmp(object_name, "gx680s") == 0) obj = new CGX680S;
				else if (strcmp(object_name, "laserguard") == 0) obj = new CLaserGuard;
				else if (strcmp(object_name, "z88") == 0) obj = new CBossZ88;
				else if (strcmp(object_name, "brick") == 0) obj = new CBrick;
				else if (strcmp(object_name, "breakable-brick") == 0) obj = new CBreakableBrick;
				else if (strcmp(object_name, "thorn") == 0) obj = new CThorn;
				else if (strcmp(object_name, "portal") == 0)
				{
					int translationX = 0;
					int translationY = 0;
					int destinationX = 0;
					int destinationY = 0;
					int sceneID = 0;

					auto props = object["properties"].GetArray();
					for (auto& prop : props)
					{
						if (strcmp(prop["name"].GetString(), "TranslationX") == 0)
						{
							translationX = prop["value"].GetInt();
						}
						else if (strcmp(prop["name"].GetString(), "TranslationY") == 0)
						{
							translationY = prop["value"].GetInt();
						}
						else if (strcmp(prop["name"].GetString(), "SceneID") == 0)
						{
							sceneID = prop["value"].GetInt();
						}
						else if (strcmp(prop["name"].GetString(), "DestinationX") == 0)
						{
							destinationX = prop["value"].GetInt();
						}
						else if (strcmp(prop["name"].GetString(), "DestinationY") == 0)
						{
							destinationY = prop["value"].GetInt();
						}
					}

					obj = new CPortal(width, height, sceneID);
					((CPortal*)obj)->SetTranslation(Vector2(translationX, translationY));
					((CPortal*)obj)->SetDestination(Vector2(destinationX, destinationY));

					portals.emplace(make_pair(sceneID, obj));

					DebugOut(L"[INFO] Portal object created!\n");
				}
				else if (strcmp(object_name, "miniportal") == 0)
				{
					int sceneID = 0;

					auto props = object["properties"].GetArray();
					for (auto& prop : props)
					{
						if (strcmp(prop["name"].GetString(), "SceneID") == 0)
						{
							sceneID = prop["value"].GetInt();
						}
					}

					obj = new CMiniPortal(width, height, sceneID);

					portals.emplace(make_pair(sceneID, obj));

					DebugOut(L"[INFO] Mini Portal object created!\n");
				}
				else if (strcmp(object_name, "boss-trigger") == 0)
				{
					int sceneID = 0;

					auto props = object["properties"].GetArray();
					for (auto& prop : props)
					{
						if (strcmp(prop["name"].GetString(), "SceneID") == 0)
						{
							sceneID = prop["value"].GetInt();
						}
					}

					obj = new CBossTrigger(width, height, sceneID);

					portals.emplace(make_pair(sceneID, obj));

					DebugOut(L"[INFO] Boss Trigger object created!\n");
				}
				else
				{
					DebugOut(L"[ERR] Invalid object type: %s\n", ToWSTR(object_name).c_str());
					continue;
				}

				if (object.HasMember("properties") == true)
				{
					auto properties = object["properties"].GetArray();
					if (properties.Size() > 0)
					{
						int nx = properties[0]["value"].GetInt();
						obj->SetDirection(nx);
					}
				}

				// [ATTENTION] For Tiled, offset of objects which are added without sprite 
				// (ex: Portal, Player,...) is top-left corner. Offset of the others is 
				// bottom-left corner.
				auto initPos = obj->GetPosition();
				obj->SetPosition(initPos + Vector2(x + width / 2, m_mapHeight - y + height / 2));

				AddGameObject(obj);
			}
		}
	}

	// Init HUD
	auto healthBar = new CHealthBar;
	HUDs.push_back(healthBar);

	fclose(fp);
}

void CPlayScene::PreSwitchingSection(CPlayScene* lastScene, Vector2 translation)
{
	// Translate object's position and push into backup vector
	/*for (auto obj : lastScene->GetUpdateObjects())
	{
		auto pos = obj->GetPosition();
		pos += translation;
		obj->SetPosition(pos);

		gameObjects_switching.emplace_back(obj);
	}*/

	background_switching = new CMapSprite(*lastScene->GetMapBackground());
	auto posMap = background_switching->GetPosition();
	posMap += translation;
	background_switching->SetPosition(posMap);

	auto mainCam = CGame::GetInstance()->GetComponent<CCamera>();

	int lastSceneID = CGame::GetInstance()->GetComponent<CScenes>()->GetLastSceneID();

	for (auto portal : portals)
	{
		if (portal.first == lastSceneID)
		{
			auto portalPos = ((CGameObject*)(portal.second))->GetPosition();
			auto destination = ((CPortal*)(portal.second))->GetDestination();
			player->SetPosition(portalPos);
			player->SetEnable(false);

			if (isTopDownView == true)
			{
				mainCam->PreUpdateSwitchingTopdownSection(destination, translation);
			}
			else
			{

				mainCam->PreUpdateSwitchingScrollingSection(destination, translation);
			}
			break;
		}
	}
}

void CPlayScene::AfterSwitchingSection()
{
	state = PlaySceneState::FreePlaying;

	/*for (auto obj : gameObjects_switching)
	{
		obj->SetEnable(false);
		obj->SetDestroyed();
	}*/

	delete background_switching;
	background_switching = nullptr;

	player->SetEnable(true);
}

void CPlayScene::HandlingInstantiateRequest()
{
	if (instantiateds.size() > 0)
	{
		for (auto obj : instantiateds)
		{
			AddGameObject(obj);
		}

		instantiateds.clear();
	}
}

void CPlayScene::Update(DWORD dt)
{
	DebugOut(L"[CODE] Update %d\n", updates.size());

	HandlingInstantiateRequest();

	auto mainCam = CGame::GetInstance()->GetComponent<CCamera>();
	mainCam->Update();

	updates.clear();
	quadtree->Update(gameObjects);
	quadtree->Retrieve(updates, mainCam->GetBoundingBox());

	// TODO: When switching section, add update object of last scene into
	// update object of current scene

	for (auto obj : updates)
		if (obj->IsEnabled() == true) obj->PhysicsUpdate(&updates);

	for (auto obj : updates)
		if (obj->IsEnabled() == true) obj->Update(dt);
}

void CPlayScene::Render()
{
	if (background != nullptr)
		background->Draw(1, 1);

	if (background_switching != nullptr)
		background_switching->Draw(1, 1);

	for (auto obj : updates)
		if (obj->IsEnabled() == true) obj->Render();

	if (foreground != nullptr)
		foreground->Draw(1, 2);

	for (auto hud : HUDs)
	{
		hud->Render();
	}

	// RENDERING GIZMO
	/*for (auto obj : updates)
		if (obj->IsEnabled() == true)
			for (auto co : obj->GetColliders())
				co->RenderBoundingBox();*/
}

/*
	Unload current scene
*/
void CPlayScene::Unload()
{
	delete background;
	background = nullptr;

	delete foreground;
	foreground = nullptr;

	for (auto obj : gameObjects)
	{
		obj->SetDestroyed();
		obj->SetEnable(false);
	}
	gameObjects.clear();

	portals.clear();

	for (auto hud : HUDs)
	{
		hud->SetDestroyed();
		hud->SetEnable(false);
	}
	HUDs.clear();

	player = nullptr;

	if (quadtree != nullptr)
	{
		//quadtree->Reset(m_mapWidth, m_mapHeight);
		delete quadtree;
		quadtree = nullptr;
	}

	DebugOut(L"[INFO] Scene %s unloaded! \n", sceneFilePath);
}

void CPlayScene::Clean()
{
	for (auto obj : gameObjects)
	{
		if (obj->IsDestroyed() == true)
		{
			auto index = find(gameObjects.begin(), gameObjects.end(), obj);
			if (index != gameObjects.end())
				gameObjects.erase(index);

			quadtree->RemoveGameObjectFromLeaf(obj);

			if (obj == player)
			{
				SetPlayer(nullptr);
			}

			delete obj;
			obj = nullptr;
		}
	}

	for (auto hud : HUDs)
	{
		if (hud->IsDestroyed() == true)
		{
			delete hud;
			hud = nullptr;
		}
	}
}

void CPlayScene::SetPlayer(CGameObject* object)
{
	player = object;
	CGame::GetInstance()->GetComponent<CCamera>()->SetTarget(object);
	CEnemy::SetTarget((CPlayable*)object);
	for (auto hud : HUDs)
	{
		dynamic_cast<CHealthBar*>(hud)->SetOwner((CPlayable*)object);
	}
}

void CPlayScene::AddGameObject(CGameObject* object)
{
	gameObjects.push_back(object);
	quadtree->Insert(object);
}

void CPlayScene::RequestInstantiate(CGameObject* object)
{
	instantiateds.emplace_back(object);
}

void CPlayScenceKeyHandler::OnKeyDown(int KeyCode)
{
	//DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);
	switch (KeyCode)
	{
	case DIK_1:
		CGame::GetInstance()->GetComponent<CScenes>()->SwitchScene(1);
		break;
	case DIK_2:
		CGame::GetInstance()->GetComponent<CScenes>()->SwitchScene(2);
		break;
	case DIK_3:
		CGame::GetInstance()->GetComponent<CScenes>()->SwitchScene(3);
		break;
	case DIK_4:
		CGame::GetInstance()->GetComponent<CScenes>()->SwitchScene(4);
		break;
	case DIK_5:
		CGame::GetInstance()->GetComponent<CScenes>()->SwitchScene(5);
		break;
	case DIK_6:
		CGame::GetInstance()->GetComponent<CScenes>()->SwitchScene(6);
		break;
	case DIK_7:
		CGame::GetInstance()->GetComponent<CScenes>()->SwitchScene(7);
		break;
	default:
		break;
	}
}

void CPlayScenceKeyHandler::KeyState(BYTE* states)
{

}