#pragma once

#include <d3dx9.h>
#include "KeyEventHandler.h"
#include "Map.h"



class CScene
{
protected:
	CKeyEventHandler * key_handler;
	int id;
	LPCWSTR sceneFilePath;

	//Bổ sung

	int type_scence;
	

public: 
	CScene(int id, LPCWSTR filePath);

	CKeyEventHandler * GetKeyEventHandler() { return key_handler; }
	virtual void Load() = 0;
	virtual void Load(int _alive, int _power) = 0;
	virtual void Unload() = 0;
	virtual void Update(DWORD dt) = 0;
	virtual void Render() = 0; 
	//Bo sung
	virtual CMap* GetMap() { return NULL; }
	virtual void GetNextMap() {  }

	void SetTypeScence(int type) { this->type_scence = type; }
	int GetTypeScence() { return this->type_scence; }

};
typedef CScene * LPSCENE;


class CScenceKeyHandler : public CKeyEventHandler
{
protected: 
	CScene * scence; 

public:
	virtual void KeyState(BYTE *states) = 0;
	virtual void OnKeyDown(int KeyCode) = 0;
	virtual void OnKeyUp(int KeyCode) = 0;
	CScenceKeyHandler(CScene *s) //:CKeyEventHandler() 
	{ scence = s; }

	
};