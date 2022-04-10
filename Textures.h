#pragma once
#include <unordered_map>

#include "Game.h"

class CTextures : public CSystem
{
	std::unordered_map<std::string, LPDIRECT3DTEXTURE9> textures;

public:
	CTextures();

	void Add(std::string id, LPCWSTR texturePath, D3DCOLOR transparentColor);
	LPDIRECT3DTEXTURE9 Get(std::string id);
	void Clear();
};