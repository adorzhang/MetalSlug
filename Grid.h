#pragma once
#include "GameObject.h"
#include "Game.h"
#include "TiledMap.h"
#define CELL_WIDTH	160
#define CELL_HEIGHT	120


class CCell
{
	vector<LPGAMEOBJECT> objects;

public:
	CCell() {  };
	void Add(LPGAMEOBJECT object) { objects.push_back(object); };
	vector<LPGAMEOBJECT> GetObjects() { return objects; }
	void Unload() { objects.clear(); }
};
//typedef CCell* LPCELL;
class CGrid
{
	int numCol;
	int numRow;
	vector<LPGAMEOBJECT> objects;
	unordered_map<int, LPGAMEOBJECT> map_objects;
	CCell** cells;

public:
	static CGrid* __instance;
	static CGrid* GetInstance();
	CGrid();
	void Init();
	void SetCol(int _numCol) { this->numCol = _numCol; }
	void SetRow(int _numRow) { this->numRow = _numRow; }
	void Add(LPGAMEOBJECT object, float x, float y);
	void Add(LPGAMEOBJECT object, int xCell, int yCell);
	//vector<LPGAMEOBJECT> GetList();
	unordered_map<int, LPGAMEOBJECT> GetList();
	void Unload();
};
