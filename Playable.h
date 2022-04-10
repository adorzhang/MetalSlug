#pragma once
#include <dinput.h>
#include "GameObject.h"

class MarcoRossi;
class SlugMariner;
class SlugNoid;

enum ControlKey
{
	MOVE_LEFT_KEY = DIK_LEFT,
	MOVE_RIGHT_KEY = DIK_RIGHT,
	JUMPING_KEY = DIK_X,
	SHOOTING_KEY = DIK_C,
	SWITCH_CHARACTER_KEY = DIK_LSHIFT,
	SOPHIA_UPWARD_KEY = DIK_UP
};

// I don't know why when I hold left and up key as the same time, I can't press Z key

class CPlayable : public CGameObject
{
protected:
	static MarcoRossi* rossi;
	static SlugMariner* mariner;
	static SlugNoid* noid;
	static DWORD switchDelay;
	static DWORD lastTimeSwitch;

	const int maxPower = 8;

	bool controllable;
	bool untouchable;
	bool onGround;

	int power;			// HP

	const DWORD untouchableTime = 1000;
	DWORD lastTimeTakeDamage;

	const DWORD changeColorTime = 30;
	DWORD lastTimeChangeColor;
public:
	CPlayable();

	bool IsInvincible() { return this->untouchable; }
	void SetControllable(bool value) { this->controllable = value; }

	int GetPower() { return this->power; }
	void SetPower(int value) { this->power = value; }

	void UntouchableUpdate();
	virtual void OnDead();

	void AffectPowerAttribute(int value);
};