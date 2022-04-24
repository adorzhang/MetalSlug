#pragma once
#include <unordered_map>
#include "Map.h"
#include "Sprites.h"
using namespace std;

class CTiledCell
{
public :
	
	LPSPRITE sprite;
public:
	CTiledCell(LPSPRITE sprite) {
		this->sprite = sprite;
	}
	
	LPSPRITE GetSprite() { return sprite; }
	~CTiledCell(){}
};
typedef CTiledCell* LPTILEDCELL;
class CTiledRow
{
public:
	int currentCell;
	vector<LPTILEDCELL> tiled_row;

	//bổ sung
	vector<LPTILEDCELL> tiled_row_next_map;
public:
	void Add(int spriteId);

	void Render(float x, float y, int alpha = 255);

};
typedef CTiledRow* LPTILEDROW;

//typedef vector<CTiledRow*> CTiledMapSet;
//typedef CTiledMapSet* LPTILEDMAP_SET;
class CTiledMap :public CMap
{
protected:
	int textureId;
	unordered_map<int, LPTILEDROW> tiledmap_row_set;
	unordered_map<int, LPTILEDROW> tiledmap_row_set_next_map;
	void _ParseSection_SPRITE_ID_CELLS(string line,int lineCount);

	void _ParseSection_MAP_WIDTH(string line);
	void _ParseSection_MAP_HEIGHT(string line);
public:
	static CTiledMap* __instance;
	CTiledMap();
	//void Add(int id, LPTILEDMAP_SET tiled_map_set);
	virtual void LoadMap(LPCWSTR filePath);
	virtual void Render();
	virtual void Render(float x, float y);
	LPTILEDROW Get(unsigned int id);
	static CTiledMap* GetInstance();
	virtual void Clear();
};
typedef CTiledMap* LPTILEDMAP;

/*
	Manage tiled map set database
*/
class CTiledMapSets
{
	static CTiledMapSets* __instance;

	//key=scene_id, value=CTiledMap object
	unordered_map<int, CMap*> tiled_map_sets;

public:
	CTiledMapSets();
	void Add(int id, CMap* map);
	CMap* Get(unsigned int id);


	static CTiledMapSets* GetInstance();
};