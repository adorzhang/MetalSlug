#include "Enemy.h"
#include "Power.h"
#include "Hover.h"
#include "Math.h"
#include "BigExplosion.h"
#include "Firework.h"
#include "Sound.h"

CPlayable* CEnemy::target = nullptr;

CEnemy::CEnemy()
{
	tag = CTag::Enemy;
	hp = maxHP;
	isTakingDamaged = false;
}

void CEnemy::DropItem()
{
	// TODO: Can only drop a single item, so convert each item's own rate to its rate in all items
	if (CMath::Random(1, 100) <= CItem::GetDropRate())
	{
		auto item = Instantiate<CPowerItem>(transform.position);
	}
}

void CEnemy::OnDestroy()
{
	DropItem();

	isEnabled = false;
	isDestroyed = true;

	if (scrollingMap == true) Instantiate<CBigExplosion>(transform.position);
	else Instantiate<CFirework>(transform.position);
}

void CEnemy::OnDamagedUpdate()
{
	if (isTakingDamaged == true)
	{
		DWORD now = GetTickCount();
		if (now - lastTimeTakeDamage > damagedTime)
		{
			isTakingDamaged = false;
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

void CEnemy::TakeDamage(int damage)
{
	CGame::GetInstance()->GetService<CSound>()->PlayWaveFile("EnemyOnDamaged");

	lastTimeTakeDamage = GetTickCount();
	isTakingDamaged = true;
	hp -= damage;
	if (hp <= 0)
	{
		hp = 0;
		OnDestroy();

		CGame::GetInstance()->GetService<CSound>()->PlayWaveFile("EnemyDie");
	}
}
