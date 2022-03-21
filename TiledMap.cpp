#include "TiledMap.h"
#include "Utils.h"
#include <iostream>
#include <fstream>
#include "Game.h"
#include "PlayScence.h"
#define MAX_MAP_LINE 1024
#define MAP_SECTION_UNKNOWN -1
#define MAP_SECTION_SPRITE_ID_CELLS 4
#define MAP_SECTION_MAP_WIDTH 2
#define MAP_SECTION_MAP_HEIGHT 3
#define TILED_MAP_SIZE 16

CTiledMap* CTiledMap::__instance = NULL;

CTiledMapSets* CTiledMapSets::__instance = NULL;
void CTiledRow::Add(int spriteId)
{
	CGame* game = CGame::GetInstance();
	//Thêm phần xử lý add sprite cho hiệu ứng render next map
	if (CGame::GetInstance()->GetRenderingNextMap())
	{
		CScene* scene_next = game->GetScene(game->GetScenceIDRenderingNextMap());
		CSprites* sprites;
		if (scene_next)
		{
			CPlayScene* play_scene = dynamic_cast<CPlayScene*>(scene_next);
			sprites = play_scene->GetSpritesNextMap();
			if (sprites)
			{
				LPSPRITE sprite = sprites->Get(spriteId);

				if (sprite == NULL)
				{
					DebugOut(L"[ERROR] Sprite ID %d cannot be found!\n", spriteId);
				}
				LPTILEDCELL cell = new CTiledCell(sprite);
				tiled_row_next_map.push_back(cell);
			}

		}
		
	}
	else
	{
		LPSPRITE sprite = CSprites::GetInstance()->Get(spriteId);

		if (sprite == NULL)
		{
			DebugOut(L"[ERROR] Sprite ID %d cannot be found!\n", spriteId);
		}
		LPTILEDCELL cell = new CTiledCell(sprite);
		tiled_row.push_back(cell);
	}
	
}
void CTiledRow::Render(float x, float y, int alpha)
{
	//for(currentCell=0;tiled_row.size();currentCell++)
		//tiled_row[currentCell]->GetSprite()->Draw(x, y, alpha);
}

CTiledMap::CTiledMap()
{

}

CTiledMap* CTiledMap::GetInstance()
{
	if (__instance == NULL) __instance = new CTiledMap();
	return __instance;
}

LPTILEDROW CTiledMap::Get(unsigned int id)
{
	LPTILEDROW tiledmap_row = tiledmap_row_set[id];
	//if (tiled_set == NULL)
		//DebugOut(L"[ERROR] Failed to find tiled map id: %d\n", id);

	return tiledmap_row;
}

//void CTiledMap::Add(int id, LPTILEDMAP_SET tiled_set)
//{
	//tiledmap_sets[id] = tiled_set;
//}
void CTiledMap::Clear()
{
	for (auto x : tiledmap_row_set)
	{
		LPTILEDROW row = x.second;
		delete row;
	}

	tiledmap_row_set.clear();
}
void CTiledMap::LoadMap(LPCWSTR filePath)
{
	this->filePath = filePath;
	bool is_MAP_SECTION_SPRITE_ID_CELLS = false;
	int lineCount = 0;
	DebugOut(L"[INFO] Start loading map file : %s\n", filePath);

	ifstream f;
	f.open(filePath);
	char str[MAX_MAP_LINE];

	// current resource section flag
	int section = MAP_SECTION_UNKNOWN;
	while (f.getline(str, MAX_MAP_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	

		if (line == "[SPRITE_ID_CELLS]") { section = MAP_SECTION_SPRITE_ID_CELLS; continue; }
		if (line == "[MAP_WIDTH]") { section = MAP_SECTION_MAP_WIDTH; continue; }
		if (line == "[MAP_HEIGHT]") { section = MAP_SECTION_MAP_HEIGHT; continue; }
		if (line[0] == '[') { section = MAP_SECTION_UNKNOWN; continue; }
		if (section == MAP_SECTION_SPRITE_ID_CELLS) is_MAP_SECTION_SPRITE_ID_CELLS = true;
		else is_MAP_SECTION_SPRITE_ID_CELLS = false;
		
		//
		// data section
		//
		switch (section)
		{
		case MAP_SECTION_SPRITE_ID_CELLS: _ParseSection_SPRITE_ID_CELLS(line,lineCount); break;
		case MAP_SECTION_MAP_WIDTH: _ParseSection_MAP_WIDTH(line); break;
		case MAP_SECTION_MAP_HEIGHT: _ParseSection_MAP_HEIGHT(line); break;
		}
		if (is_MAP_SECTION_SPRITE_ID_CELLS)
			lineCount++;
	}

	f.close();
}

void CTiledMap::_ParseSection_SPRITE_ID_CELLS(string line,int lineCount)
{
	vector<string> tokens = split(line,",");

	if (tokens.size() < 1) return; // skip invalid lines
	LPTILEDROW row = new CTiledRow();
	for (int i = 0; i < tokens.size(); i += 1)	
	{
		int spriteId = atoi(tokens[i].c_str());
		
		row->Add(spriteId);
	}
	//Bổ sung code cho hiệu ứng vẽ scence tiếp theo trước khi switch_scence
	if (CGame::GetInstance()->GetRenderingNextMap())
	{
		tiledmap_row_set_next_map[lineCount] = row;
	}
	else
		tiledmap_row_set[lineCount]= row;
}

void CTiledMap::_ParseSection_MAP_WIDTH(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 1) return; // skip invalid lines

	int map_width = atoi(tokens[0].c_str());
	this->map_width = map_width;
}
void CTiledMap::_ParseSection_MAP_HEIGHT(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 1) return; // skip invalid lines

	int map_height = atoi(tokens[0].c_str());
	this->map_height = map_height;
}

void CTiledMap::Render()
{
	for (int r_index = 0; r_index < tiledmap_row_set.size(); r_index++)
	{

		for (int c_index = 0; c_index < tiledmap_row_set[r_index]->tiled_row.size(); c_index++)
		{
			LPTILEDCELL cell = tiledmap_row_set[r_index]->tiled_row[c_index];
			cell->GetSprite()->Draw(c_index > 0 ? c_index * TILED_MAP_SIZE : c_index * TILED_MAP_SIZE, r_index == 0 ? r_index * this->map_height : this->map_height-r_index * TILED_MAP_SIZE);
		}
	}
}

void CTiledMap::Render(float x, float y)
{
	for (int r_index = 0; r_index < tiledmap_row_set_next_map.size(); r_index++)
	{

		for (int c_index = 0; c_index < tiledmap_row_set_next_map[r_index]->tiled_row_next_map.size(); c_index++)
		{
			LPTILEDCELL cell = tiledmap_row_set_next_map[r_index]->tiled_row_next_map[c_index];
			cell->GetSprite()->Draw(c_index * TILED_MAP_SIZE+x, r_index == 0 ? r_index * this->map_height+y : this->map_height - r_index * TILED_MAP_SIZE+y);
		}
	}
}

CTiledMapSets::CTiledMapSets()
{

}

CTiledMapSets* CTiledMapSets::GetInstance()
{
	if (__instance == NULL) __instance = new CTiledMapSets();
	return __instance;
}

CMap* CTiledMapSets::Get(unsigned int id)
{
	CMap* map = tiled_map_sets[id];
	if (map == NULL)
		DebugOut(L"[ERROR] Failed to find map set id: %d\n", id);
	return map;
}

void CTiledMapSets::Add(int id, CMap* map)
{
	tiled_map_sets[id] = map;
}
