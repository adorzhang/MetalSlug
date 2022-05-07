#pragma once
using namespace std;
#include <Windows.h>
#include <unordered_map>
class CMap;
typedef CMap * LPMAP;
class CMap {
public:
	int map_width;
	int map_height;
	LPCWSTR filePath;
	vector<string> background;
	vector<string> foreground;

public:
	CMap();
	void SetMapWidth(int map_width) { this->map_width = map_width; }
	void SetMapHeight(int map_height) { this->map_height = map_height; }
	void GetMapWidth(int &map_width) {map_width= this->map_width ; }
	void GetMapHeight(int &map_height) { map_height=this->map_height ; }

	virtual void LoadMap(LPCWSTR filePath) {};
	virtual void Update(DWORD dt) {};
	virtual void Render() {};
	virtual void Render(float x, float y) {};
	vector<string> getBackground() { return background; };
	vector<string>  getForeground() { return foreground; };

	virtual void Clear() {};
	~CMap(){}
};