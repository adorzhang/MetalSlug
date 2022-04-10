#pragma once
#include <unordered_map>
#include "Textures.h"

class CSprite
{
protected:
	int left;
	int top;
	int width;
	int height;

	LPDIRECT3DTEXTURE9 texture;

public:
	CSprite(int left, int top, int witdh, int height, LPDIRECT3DTEXTURE9 texture);

	void Draw(Vector2 position, int nx, int layer_index, float angle = 0.0f, D3DCOLOR color = D3DCOLOR_ARGB(255, 255, 255, 255));
};

typedef CSprite* LPSPRITE;

class CSprites : public CSystem
{
	std::unordered_map<std::string, LPSPRITE> sprites;

public:
	void Add(std::string id, int left, int top, int width, int height, LPDIRECT3DTEXTURE9 texture);
	LPSPRITE Get(std::string id);
	void Clear();
};

class CMapSprite
{
protected:
	int width;
	int height;

	Vector2 position;

	LPDIRECT3DTEXTURE9 texture;
public:
	CMapSprite(int witdh, int height, LPDIRECT3DTEXTURE9 texture);
	CMapSprite(const CMapSprite& spr);
	Vector2 GetPosition() { return this->position; }
	void SetPosition(Vector2 pos) { this->position = pos; }
	void Draw(int nx, int layer_index, D3DCOLOR color = D3DCOLOR_ARGB(255, 255, 255, 255));
};

typedef CMapSprite* LPMAPSPRITE;