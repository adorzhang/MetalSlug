#include "Item.h"
#include "Sound.h"

int CItem::dropRate = 50;

CItem::CItem()
{
	// Init colliders
	auto collider = new CCollider2D;
	collider->SetGameObject(this);
	collider->SetTrigger(true);
	collider->SetDynamic(true);
	colliders.push_back(collider);

	CTag = CTag::Item;

	timeSpawn = GetTickCount();
	flickeringDuration = (aliveDuration - preWarningDuration) / flickeringTimes;
}

void CItem::GetEffect(CPlayable* picker)
{
}

void CItem::Update(DWORD dt)
{
	DWORD now = GetTickCount();

	if (now - timeSpawn > preWarningDuration)
	{
		int index = (now - timeSpawn - preWarningDuration) / flickeringDuration % alphaArray.size();
		alpha = alphaArray[index];
	}

	if (now - timeSpawn > aliveDuration)
	{
		SetDestroyed();
		SetEnable(false);
	}
}

void CItem::Render()
{
}

void CItem::OnTriggerEnter(CCollider2D* selfCollider, CCollisionEvent* collision)
{
	auto other = collision->obj;
	if (dynamic_cast<CPlayable*>(other))
	{
		GetEffect((CPlayable*)other);
		SetDestroyed();
		SetEnable(false);

		CGame::GetInstance()->GetComponent<CSound>()->PlayWaveFile("PickItemUp");
	}
}
