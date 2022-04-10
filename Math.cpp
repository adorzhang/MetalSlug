#include "Math.h"

const float CMath::PI = 3.14f;

int CMath::Random(int start, int end)
{
	return rand() % (end - start + 1) + start;
}

float CMath::CalcDistance(Vector2 pos1, Vector2 pos2)
{
	return sqrt((pow(pos1.x - pos2.x, 2) + pow(pos1.y - pos2.y, 2)));
}


float CMath::CalcLength(Vector2 v)
{
	return sqrt(pow(v.x, 2) + pow(v.y, 2));
}

Vector2 CMath::Rotating(Vector2 v, float angle)
{
	float theta = angle * 3.14 / 180;

	float cs = cos(theta);
	float sn = sin(theta);

	float px = v.x * cs - v.y * sn;
	float py = v.x * sn + v.y * cs;

	return Vector2(px, py);
}


Vector2 CMath::Normalize(Vector2 v)
{
	Vector2 result = VectorZero();
	float length = CalcLength(v);

	if (length != 0)
	{
		result.x = v.x / length;
		result.y = v.y / length;
	}

	return result;
}