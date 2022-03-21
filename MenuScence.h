#pragma once
#pragma once
#include "Game.h"
#include "Textures.h"
#include "Scence.h"
#include "GameObject.h"
class CMenuScence : public CScene
{
protected:
	void _ParseSection_TEXTURES(string line);
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_ANIMATION_SETS(string line);
	void _ParseSection_OBJECTS(string line);
	int scence_id=-1;
	int alives;
	int power;
	float x_button;
	float y_button;
	//bool isContinueButton;
	//bool isCancelButton;
public:

	CMenuScence(int id, LPCWSTR filePath);

	virtual void Load() {};
	virtual void Load(int _alive = 0, int _power = 0);
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();
	void SetScenceId(int _id) { scence_id = _id; }
	int GetScenceId() { return scence_id; }
};

class CMenuScenceKeyHandler : public CScenceKeyHandler
{
public:
	virtual void KeyState(BYTE* states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);
	CMenuScenceKeyHandler(CScene* s) :CScenceKeyHandler(s) {};
};

