#include "Grid.h"

CGrid* CGrid::__instance = NULL;
CGrid* CGrid::GetInstance()
{
	if (__instance == NULL) __instance = new CGrid();
	return __instance;
}
CGrid::CGrid()
{
	objects.clear();
}

void CGrid::Init()
{
	cells = new CCell* [numRow];
	for (int i = 0; i < numRow; i++)
		cells[i] = new CCell[numCol];
	
}

void CGrid::Add(LPGAMEOBJECT object, float x, float y)
{
	
}

void CGrid::Add(LPGAMEOBJECT object, int xCell, int yCell)
{
	cells[xCell][yCell].Add(object);
}

unordered_map<int,LPGAMEOBJECT> CGrid::GetList()
{
	int heightMap;
	
	CGame* game = CGame::GetInstance();
	CTiledMapSets::GetInstance()->Get(game->GetCurrentScenceID())->GetMapHeight(heightMap);
	
	float x, y;
	game->GetCamPos(x,y);
	

	int startX = x / CELL_WIDTH;
	int endX = (x + game->GetScreenWidth()) / CELL_WIDTH;

	int startY =(heightMap - y) / CELL_HEIGHT;
	int endY = (heightMap -y + game->GetScreenHeight()) / CELL_HEIGHT;
	if (endY == numRow)
		endY -= 1;
	if (endX == numCol)
		endX -= 1;
	if (startX >= numCol || startY >= numRow || endX >= numCol || endY >= numRow)
		return map_objects;
		//return objects;
	map_objects.clear();
	for (int i = startY; i <= endY; i++)
	{
		for (int j = startX; j <= endX; j++)
		{
			if (cells[i][j].GetObjects().size() > 0)
			{
				for (int k = 0; k < cells[i][j].GetObjects().size(); k++)
				{
					if (cells[i][j].GetObjects().at(k) != NULL)
					{
						LPGAMEOBJECT object = cells[i][j].GetObjects().at(k);
						int id = object->GetID();
						//objects.push_back(cells[i][j].GetObjects().at(k));
						map_objects[id] = object;
					}
				}
			}
		}
	}
	//return objects;
	return map_objects;
}

void CGrid::Unload()
{
	for (int i = 0; i < numRow; i++)
	{
		for (int j = 0; j < numCol; j++)
		{
			cells[i][j].Unload();
		}
	}

	objects.clear();
}