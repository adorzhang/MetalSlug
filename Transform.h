#pragma once
#include <d3dx9.h>
typedef D3DXVECTOR2 Vector2;
typedef D3DXVECTOR3 Vector3;

class Transform
{
public:
	Vector2 position;
	Vector2 rotation;
	Vector2 scale;
};

struct Rect
{
	float left, top, right, bottom;
	Rect();
	Rect(float l, float t, float r, float b) { left = l; top = t; right = r; bottom = b; }
	bool Equal(const Rect& other);
	bool Overlap(const Rect& other);
	bool Contain(const Rect& other) const;
	bool Contain(const Vector2& point) const;
	Rect RectZero() { return { 0, 0, 0, 0 }; }
};

Vector2 VectorInfinity();
Vector2 VectorZero();
Vector2 NormalizedVector();