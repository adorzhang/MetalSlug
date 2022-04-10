#include "Textures.h"

CTextures::CTextures()
{

}

void CTextures::Add(std::string id, LPCWSTR texturePath, D3DCOLOR transparentColor)
{
	auto newTex = CGame::GetInstance()->LoadTexture(texturePath, transparentColor);
	textures.insert(make_pair(id, newTex));
}

LPDIRECT3DTEXTURE9 CTextures::Get(std::string id)
{
	return textures.at(id);
}

void CTextures::Clear()
{
	for (auto x : textures)
	{
		LPDIRECT3DTEXTURE9 tex = x.second;
		if (tex != NULL) tex->Release();
	}

	textures.clear();
}