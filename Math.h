#pragma once
#include <stdlib.h>
#include "Transform.h"

class CMath
{
public:
	static const float PI;

	static int Random(int start, int end);	// Get random integer number between start to end
	static float CalcDistance(Vector2 pos1, Vector2 pos2);
	static float CalcLength(Vector2 v);
	static Vector2 Normalize(Vector2 v);
	static Vector2 Rotating(Vector2 v, float angle);
};