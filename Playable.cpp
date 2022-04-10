#include "Playable.h"

MarcoRossi* CPlayable::rossi = nullptr;
SlugMariner* CPlayable::mariner = nullptr;
SlugNoid* CPlayable::noid = nullptr;

DWORD CPlayable::switchDelay = 300;
DWORD CPlayable::lastTimeSwitch = 0;

CPlayable::CPlayable()
{
	power = maxPower;
}

void CPlayable::OnDead()
{

}

void CPlayable::UntouchableUpdate()
{
	if (untouchable == true)
	{
		DWORD now = GetTickCount();
		if (now - lastTimeTakeDamage > untouchableTime)
		{
			untouchable = false;
			colorIndex = 0;
		}
		else
		{
			if (now - lastTimeChangeColor > changeColorTime)
			{
				lastTimeChangeColor = now;
				colorIndex++;
				if (colorIndex >= damagedColor.size()) colorIndex = 0;
			}
		}
	}
}

void CPlayable::AffectPowerAttribute(int value)
{
	if (untouchable == true && value < 0) return;

	power += value;
	if (value < 0)
	{
		lastTimeTakeDamage = GetTickCount();
		untouchable = true;
	}

	if (power <= 0)
	{
		power = 0;
		OnDead();
	}

	if (power >= maxPower)
	{
		power = maxPower;
	}
}
