#include <fstream>
#include "SceneManager.h"
#include "Utils.h"
#include "PlayScene.h"
#include "Animation.h"

void CSceneManager::_ParseSection_SETTINGS(std::string line)
{
	std::vector<std::string> tokens = split(line);

	if (tokens.size() < 2) return;
	if (tokens[0] == "start")
		current_scene = atoi(tokens[1].c_str());
	else
		DebugOut(L"[ERROR] Unknown game setting %s\n", ToWSTR(tokens[0]).c_str());
}

void CSceneManager::_ParseSection_SCENES(std::string line)
{
	std::vector<std::string> tokens = split(line);

	if (tokens.size() < 2) return;
	int id = atoi(tokens[0].c_str());
	LPCWSTR path = ToLPCWSTR(tokens[1]);

	LPSCENE scene = new CPlayScene(id, path);
	scenes[id] = scene;
}

void CSceneManager::Load(LPCWSTR gameFile)
{
	DebugOut(L"[INFO] Start loading game file : %s\n", gameFile);

	ifstream f;
	f.open(gameFile);
	char str[MAX_GAME_LINE];

	// current resource section flag
	int section = GAME_FILE_SECTION_UNKNOWN;

	while (f.getline(str, MAX_GAME_LINE))
	{
		std::string line(str);

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

	DebugOut(L"[INFO] Loading game file : %s has been loaded successfully\n", gameFile);

	SwitchScene(current_scene);
}

void CSceneManager::SwitchScene(int scene_id)
{
	DebugOut(L"[INFO] Switching to scene %d\n", scene_id);

	scenes[current_scene]->Unload();

	auto game = CGame::GetInstance();

	game->GetSystem<CTextures>()->Clear();
	game->GetSystem<CSprites>()->Clear();
	game->GetSystem<CAnimation>()->Clear();

	last_scene = current_scene;
	current_scene = scene_id;
	auto s = (CPlayScene*)scenes[scene_id];

	game->GetSystem<CInputHandler>()->SetKeyHandler(s->GetKeyEventHandler());
	s->Load();

	for (auto portal : s->GetPortalList())
	{
		if (portal.first == last_scene)
		{
			auto portalPos = ((CGameObject*)(portal.second))->GetPosition();
			s->GetPlayer()->SetPosition(portalPos);
			break;
		}
	}

	auto mainCam = game->GetSystem<CCamera>();
	if (s->IsTopDownView() == true)
	{
		mainCam->SetState(CameraState::FreePlaying_TopDown);
	}
	else
	{
		mainCam->SetState(CameraState::FreePlaying_Scrolling);
	}
}

void CSceneManager::SwitchSection(int scene_id, Vector2 translation)
{
	DebugOut(L"[INFO] Switching to section %d\n", scene_id);

	auto game = CGame::GetInstance();

	last_scene = current_scene;
	current_scene = scene_id;
	auto s = (CPlayScene*)scenes[scene_id];
	game->GetSystem<CInputHandler>()->SetKeyHandler(s->GetKeyEventHandler());

	s->SetState(PlayState::Switching);
	s->Load();
	s->PreSwitchingSection((CPlayScene*)scenes[last_scene], translation);

	scenes[last_scene]->Unload();
}
