#pragma once
#include "GameObject.h"

/*
	Object that triggers scene switching
*/

class CPortal : public CGameObject
{
	Vector2 translation;	// translation position of game object and background when switch section
	int scene_id;			// target scene to switch to 

	int width;
	int height;
	Vector2 destination;
public:
	CPortal(float w, float h, int scene_id);
	Vector2 GetTranslation() { return this->translation; }
	void SetTranslation(Vector2 trans) { this->translation = trans; }
	Vector2 GetDestination() { return this->destination; }
	void SetDestination(Vector2 position) { this->destination = position; }
	int GetSceneId() { return scene_id; }

	void Update(DWORD dt);
	void Render();

	void OnTriggerEnter(CCollider2D* selfCollider, CCollisionEvent* collision);
};