#pragma once
#include <memory>
#include <unordered_map>

#include "Game.h"
#include "Textures.h"
#include "Scene.h"
#include "InputHandler.h"
#include "Camera.h"
#include "Sprites.h"
#include "Quadtree.h"
#include "SceneManager.h"

const int CELL_SIZE = 128;

class CGameObject;
class CQuadtree;

enum class State
{
	FreePlaying,
	Switching,
};

class CPlayScene : public CScene
{
protected:
	State state;

	CGameObject* player;
	std::unordered_map<int, CGameObject*> portals;

	LPMAPSPRITE background;
	LPMAPSPRITE foreground;
	std::vector<CGameObject*> gameObjects;
	std::vector<CGameObject*> instantiateds, destroyeds;

	std::vector<CGameObject*> updates;
	CQuadtree* quadtree;					// Quadtree for space partitioning

	LPMAPSPRITE background_switching;
	LPMAPSPRITE foreground_switching;					// Unnessary
	std::vector<CGameObject*> gameObjects_switching;	//

	std::vector<CGameObject*> HUDs;

	bool isTopDownView;
	float m_mapWidth;
	float m_mapHeight;

	void _ParseSection_TEXTURES(std::string line);
	void _ParseSection_SPRITES(std::string line);
	void _ParseSection_ANIMATIONS(std::string line);
	void _ParseSection_MAP(std::string line);

public:
	CPlayScene(int id, LPCWSTR filePath);
	~CPlayScene();

	virtual void Load();
	virtual void PreSwitchingSection(CPlayScene* lastScene, Vector2 translation);
	virtual void AfterSwitchingSection();

	virtual void HandlingInstantiateRequest();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();
	virtual void Clean();

	void SetState(PlaySceneState _state) { this->state = _state; }
	bool IsTopDownView() { return this->isTopDownView; }
	LPMAPSPRITE GetMapBackground() { return this->background; }
	CGameObject* GetPlayer() { return player; }
	void SetPlayer(CGameObject* object);
	std::unordered_map<int, CGameObject*> GetPortalList() { return this->portals; }
	std::vector<CGameObject*> GetUpdateObjects() { return this->updates; }

	void AddGameObject(CGameObject* object);
	void RequestInstantiate(CGameObject* object);
};

template<typename T>
inline T* Instantiate(Vector2 position)
{
	CGameObject* newObject = new T;
	newObject->SetPosition(position);
	auto current_scene = (CPlayScene*)CGame::GetInstance()->GetComponent<CScenes>()->GetCurrentScene();
	current_scene->RequestInstantiate(newObject);
	return (T*)newObject;
}

class CPlayScenceKeyHandler : public CScenceKeyHandler
{
public:
	virtual void KeyState(BYTE* states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode) {};
	CPlayScenceKeyHandler(CScene* s) :CScenceKeyHandler(s) {};
};