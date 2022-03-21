#pragma once

#include "GameObject.h"

/*
	Object that triggers scene switching
*/
class CPortal : public CGameObject
{
	int scene_id;	// target scene to switch to 

	int width;
	int height; 
	int type;

	int next_portal_id ;
public:
	CPortal(float l, float t, float r, float b, int scene_id,int type,int next_portal_id);
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
	int GetSceneId() { return scene_id;  }
	int GetType() { return type; }
	int GetNextPortalId() { return next_portal_id; }
};