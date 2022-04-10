#pragma once

enum class CTag
{
	Player,
	Mariner,
	Rossi,
	Noid,

	Enemy,
	BossZ88,

	Item,

	PlayerBullet,
	EnemyBullet,

	Platform,
	HarmPlatform,

	Portal,
	MiniPortal,
	BossTrigger,

	Unknown,
};

class CTagUtils
{
	// Get groups of object CTag for purposes
public:
	static bool PlayerCTag(CTag CTag);
	static bool EnemyCTag(CTag CTag);
};

