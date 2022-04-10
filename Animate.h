#pragma once
#include "Frame.h"

class CAnimate
{
	std::vector<LPANIMATION_FRAME> frames;

	int defaultFrame;
	int currentFrame;
	DWORD lastFrameTime;

	bool isLooped = true;
	bool isPaused = false;
	bool isReversed = false;
	bool isFinished = false;

public:
	CAnimate();
	CAnimate(const CAnimate& anim);
	void Add(std::string id, DWORD time);
	void Render(Vector2 position, int nx, int layer_index, float angle = 0.0f, D3DCOLOR color = D3DCOLOR_ARGB(255, 255, 255, 255));

	int GetCurrentFrame() { return this->currentFrame; }
	void SetIsLooped(bool value) { this->isLooped = value; }
	void SetIsPaused(bool value) { this->isPaused = value; }
	void SetIsReversed(bool value) { this->isReversed = value; }
	bool IsFinished() { return this->isFinished; }
	void SetIsFinished(bool value) { this->isFinished = value; }
};

typedef CAnimate* LPANIMATION;