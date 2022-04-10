#include "Sprites.h"
#include "Utils.h"

CSprite::CSprite(int l, int t, int w, int h, LPDIRECT3DTEXTURE9 tex)
{
	this->left = l;
	this->top = t;
	this->width = w;
	this->height = h;
	this->texture = tex;
}

void CSprite::Draw(Vector2 position, int nx, int layer_index, float angle, D3DCOLOR color)
{
	CGame::GetInstance()->Draw(position, nx, layer_index, texture, left, top, left + width, top + height, angle, color);
}

void CSprites::Add(std::string id, int left, int top, int width, int height, LPDIRECT3DTEXTURE9 texture)
{
	sprites.insert(make_pair(id, new CSprite(left, top, width, height, texture)));
	/*DebugOut(L"[INFO] sprite added: %s\t%d\t%d\t%d\t%d\n", ToWSTR(id).c_str(), left, top, height, width);*/
}

LPSPRITE CSprites::Get(std::string id)
{
	return sprites.at(id);
}

void CSprites::Clear()
{
	for (auto x : sprites)
	{
		LPSPRITE s = x.second;
		delete s;
	}

	sprites.clear();
}

CMapSprite::CMapSprite(int w, int h, LPDIRECT3DTEXTURE9 tex)
{
	this->width = w;
	this->height = h;
	this->texture = tex;
	this->position = VectorZero();
}

CMapSprite::CMapSprite(const CMapSprite& spr)
{
	this->width = spr.width;
	this->height = spr.height;
	this->texture = spr.texture;
	this->position = spr.position;
}

void CMapSprite::Draw(int nx, int layer_index, D3DCOLOR color)
{
	CGame::GetInstance()->Draw(position, nx, layer_index, texture, 0, 0, width, height, 0, color);
}
