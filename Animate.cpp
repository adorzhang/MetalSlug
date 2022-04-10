#include "Animate.h"
#include "Utils.h"

CAnimate::CAnimate()
{
	currentFrame = -1;
}

CAnimate::CAnimate(const CAnimate& anim)
{
	this->frames = anim.frames;
	currentFrame = -1;
}

void CAnimate::Add(std::string id, DWORD time)
{
	LPSPRITE newSprite = CGame::GetInstance()->GetComponent<CSprites>()->Get(id);
	LPANIMATION_FRAME frame = new CAnimationFrame(newSprite, time);
	frames.push_back(frame);
	/*DebugOut(L"[INFO] animation frame added: %s\t%d\n", ToWSTR(id).c_str(), frames.size());*/
}

void CAnimate::Render(Vector2 position, int nx, int layer_index, float angle, D3DCOLOR color)
{
	DWORD now = GetTickCount();

	if (isReversed == false)
	{
		if (currentFrame == -1)
		{
			currentFrame = 0;
			lastFrameTime = now;
		}
		else
		{
			DWORD t = frames[currentFrame]->GetTime();
			if (now - lastFrameTime > t)
			{
				if (isPaused == false) currentFrame++;
				lastFrameTime = now;

				if (currentFrame == frames.size())
				{
					if (isLooped == true) currentFrame = 0;
					else
					{
						isFinished = true;
						currentFrame--;
					}
				}
			}
		}
	}
	else
	{
		if (currentFrame == -1)
		{
			currentFrame = frames.size() - 1;
			lastFrameTime = now;
		}
		else
		{
			DWORD t = frames[currentFrame]->GetTime();
			if (now - lastFrameTime > t)
			{
				if (isPaused == false) currentFrame--;
				lastFrameTime = now;

				if (currentFrame == -1)
				{
					if (isLooped == true) currentFrame = frames.size() - 1;
					else
					{
						isFinished = true;
						currentFrame++;
					}
				}
			}
		}
	}

	frames[currentFrame]->GetSprite()->Draw(position, nx, layer_index, angle, color);
}