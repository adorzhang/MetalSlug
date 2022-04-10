#include "Transform.h"

Vector2 VectorInfinity()
{
	return Vector2(999999.0f, 999999.0f);
}

Vector2 VectorZero()
{
	return Vector2(0.0f, 0.0f);
}

Vector2 NormalizedVector()
{
	return Vector2(1.0f, 1.0f);
}

Rect::Rect()
{
}

bool Rect::Equal(const Rect& other)
{
	if (this->left == other.left && this->top == other.top &&
		this->right == other.right && this->bottom == other.bottom)
		return true;
	else return false;
}

bool Rect::Overlap(const Rect& other)
{
	if (this->right < other.left || this->left > other.right)
		return false;
	if (this->top < other.bottom || this->bottom > other.top)
		return false;
	return true;
}

bool Rect::Contain(const Rect& other) const
{
	return this->left <= other.left &&
		this->top >= other.top &&
		this->right >= other.right &&
		this->bottom <= other.bottom;
}

bool Rect::Contain(const Vector2& point) const
{
	return this->left <= point.x &&
		this->top >= point.y &&
		this->right >= point.x &&
		this->bottom <= point.y;
}