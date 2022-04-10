#include <algorithm>    
#include "Collider2D.h"
#include "Utils.h"
#include "Sophia.h"
#include "GX680.h"

void CCollider2D::SweptAABB(
	Rect movingRect, Rect staticRect,
	float dx, float dy,
	float& nx, float& ny, float& t)
{
	float dx_entry, dx_exit, tx_entry, tx_exit;
	float dy_entry, dy_exit, ty_entry, ty_exit;

	float t_entry, t_exit;

	t = -1.0f;			// no collision
	nx = ny = 0.0f;

	//
	// Broad-phase test 
	//

	Rect rBroadPhase;
	rBroadPhase.left = dx > 0 ? movingRect.left : movingRect.left + dx;
	rBroadPhase.top = dy > 0 ? movingRect.top + dy : movingRect.top;
	rBroadPhase.right = dx > 0 ? movingRect.right + dx : movingRect.right;
	rBroadPhase.bottom = dy > 0 ? movingRect.bottom : movingRect.bottom + dy;

	if (!rBroadPhase.Overlap(staticRect)) return;

	if (dx == 0 && dy == 0) return;		// moving object is not moving > obvious no collision

	if (dx > 0)
	{
		dx_entry = staticRect.left - movingRect.right;
		dx_exit = staticRect.right - movingRect.left;
	}
	else if (dx < 0)
	{
		dx_entry = staticRect.right - movingRect.left;
		dx_exit = staticRect.left - movingRect.right;
	}

	if (dy > 0)
	{
		dy_entry = staticRect.bottom - movingRect.top;
		dy_exit = staticRect.top - movingRect.bottom;
	}
	else if (dy < 0)
	{
		dy_entry = staticRect.top - movingRect.bottom;
		dy_exit = staticRect.bottom - movingRect.top;
	}

	if (dx == 0)
	{
		tx_entry = -VectorInfinity().x;
		tx_exit = VectorInfinity().x;
	}
	else
	{
		tx_entry = dx_entry / dx;
		tx_exit = dx_exit / dx;
	}

	if (dy == 0)
	{
		ty_entry = -VectorInfinity().y;
		ty_exit = VectorInfinity().y;
	}
	else
	{
		ty_entry = dy_entry / dy;
		ty_exit = dy_exit / dy;
	}

	if ((tx_entry < 0.0f && ty_entry < 0.0f) || tx_entry > 1.0f || ty_entry > 1.0f) return;

	t_entry = max(tx_entry, ty_entry);
	t_exit = min(tx_exit, ty_exit);

	if (t_entry > t_exit) return;

	t = t_entry;

	if (tx_entry > ty_entry)
	{
		ny = 0.0f;
		dx > 0 ? nx = -1.0f : nx = 1.0f;
	}
	else
	{
		nx = 0.0f;
		dy > 0 ? ny = -1.0f : ny = 1.0f;
	}
}

/*
	Extension of original SweptAABB to deal with two moving objects
*/
LPCOLLISIONEVENT CCollider2D::SweptAABBEx(CCollider2D* coOther)
{
	Rect staticRect;		// static object bbox
	Rect movingRect;		// moving object bbox
	float t, nx, ny;

	float dt = CGame::GetDeltaTime();

	staticRect = coOther->GetBoundingBox();

	// deal with moving object: m speed = original m speed - collide object speed
	auto coObject = coOther->GetGameObject();
	float sdx = coObject->GetVelocity().x * dt;
	float sdy = coObject->GetVelocity().y * dt;

	auto velSelf = object->GetVelocity();
	this->dx = velSelf.x * dt;
	this->dy = velSelf.y * dt;

	// (rdx, rdy) is RELATIVE movement distance/velocity 
	float rdx = dx - sdx;
	float rdy = dy - sdy;

	movingRect = GetBoundingBox();

	SweptAABB(
		movingRect, staticRect,
		rdx, rdy, nx, ny, t);

	CCollisionEvent* e = new CCollisionEvent(t, nx, ny, coObject, coOther);
	return e;
}

/*
	Calculate potential collisions with the list of colliable objects

	coObjects: the list of colliable objects
	coEvents: list of potential collisions
*/
void CCollider2D::CalcPotentialCollisions(
	std::vector<CGameObject*>* coObjects,
	std::vector<LPCOLLISIONEVENT>& coEvents)
{
	for (UINT i = 0; i < coObjects->size(); i++)
	{
		if (object == coObjects->at(i)) continue;
		if (coObjects->at(i)->IsEnabled() == false) continue;
		if (coObjects->at(i)->GetColliders().size() == 0) continue;

		auto selfCTag = object->GetCTag();
		auto otherCTag = coObjects->at(i)->GetCTag();
		if ((selfCTag == otherCTag && selfCTag != CTag::BossZ88) ||
			(CTagUtils::EnemyCTag(selfCTag) && otherCTag == CTag::EnemyBullet) ||
			(CTagUtils::EnemyCTag(otherCTag) && selfCTag == CTag::EnemyBullet) ||
			(CTagUtils::PlayerCTag(selfCTag) && otherCTag == CTag::PlayerBullet) ||
			(CTagUtils::PlayerCTag(otherCTag) && selfCTag == CTag::PlayerBullet) ||
			(selfCTag == CTag::EnemyBullet && otherCTag == CTag::PlayerBullet) ||
			(otherCTag == CTag::EnemyBullet && selfCTag == CTag::PlayerBullet)) continue;

		for (auto co : coObjects->at(i)->GetColliders())
		{
			LPCOLLISIONEVENT e = SweptAABBEx(co);

			if (e->t > 0 && e->t <= 1.0f)
				coEvents.push_back(e);
			else
				delete e;
		}
	}

	std::sort(coEvents.begin(), coEvents.end(), CCollisionEvent::compare);
}

void CCollider2D::FilterCollision(
	std::vector<LPCOLLISIONEVENT>& coEvents,
	LPCOLLISIONEVENT& colX,
	LPCOLLISIONEVENT& colY,
	bool filterX = true,
	bool filterY = true)
{
	float min_tx, min_ty;

	min_tx = 1.0f;
	min_ty = 1.0f;
	int min_ix = -1;
	int min_iy = -1;

	for (UINT i = 0; i < coEvents.size(); i++)
	{
		LPCOLLISIONEVENT c = coEvents[i];

		if (c->isDeleted == true) continue;
		if (c->co->IsTrigger() == true) continue;

		auto selfCTag = object->GetCTag();
		auto otherCTag = coEvents[i]->obj->GetCTag();
		if ((CTagUtils::PlayerCTag(selfCTag) && CTagUtils::EnemyCTag(otherCTag)) ||
			(CTagUtils::PlayerCTag(otherCTag) && CTagUtils::EnemyCTag(selfCTag)))
		{
			continue;
		}

		if (c->t < min_tx && c->nx != 0 && filterX == true) {
			min_tx = c->t; min_ix = i;
		}

		if (c->t < min_ty && c->ny != 0 && filterY == true) {
			min_ty = c->t; min_iy = i;
		}
	}

	if (min_ix >= 0) colX = coEvents[min_ix];
	if (min_iy >= 0) colY = coEvents[min_iy];
}

void CCollider2D::PhysicsUpdate(std::vector<CGameObject*>* coObjects)
{
	// Reduce the number of check collision if game object is not enabled, is nullptr, static
	if (object == nullptr || isDynamic == false) return;

	auto dt = CGame::GetDeltaTime();
	auto pos = object->GetPosition();
	auto velocity = object->GetVelocity();

	this->dx = velocity.x * dt;
	this->dy = velocity.y * dt;

	coEvents.clear();
	coEventX = NULL;
	coEventY = NULL;

	float min_t = 1.0f;

	CalcPotentialCollisions(coObjects, coEvents);

	if (coEvents.size() == 0)
	{
		//if (dynamic_cast<CGX680*>(object)) DebugOut(L"size 0\n");

		pos.x += dx;
		pos.y += dy;

		object->SetPosition(pos);
	}
	else
	{
		FilterCollision(coEvents, coEventX, coEventY);

		if (coEventX != NULL && coEventY != NULL)
		{
			// was collision on Y first ?
			if (coEventY->t < coEventX->t)
			{
				//if (dynamic_cast<CSophia*>(object)) DebugOut(L"Y first\n");

				min_t = coEventY->t;

				PushingHandling(coEventY, false, 1);

				if (coEventY->ny != 0)
					if (isTrigger == false) object->OnCollisionEnter(this, coEventY);
					else object->OnTriggerEnter(this, coEventY);

				//
				// see if after correction on Y, is there still a collision on X ? 
				//
				LPCOLLISIONEVENT colX_other = NULL;

				//
				// check again if there is true collision on X 
				//

				// remove current collision event on X
				coEventX->isDeleted = true;

				// replace with a new collision event using corrected location 
				coEvents.push_back(SweptAABBEx(coEventX->co));

				// re-filter on X only
				FilterCollision(coEvents, colX_other, coEventY, /*filterX=*/1, /*filterY=*/0);

				if (colX_other != NULL)
				{
					PushingHandling(colX_other, false, coEventY->t);

					if (isTrigger == false) object->OnCollisionEnter(this, colX_other);
					else object->OnTriggerEnter(this, colX_other);
				}
				else
				{
					pos.x += coEventY->t * dx;
					object->SetPosition(pos);
				}
			}
			// collision on X first
			else
			{
				//if (dynamic_cast<CSophia*>(object)) DebugOut(L"X first\n");

				min_t = coEventX->t;

				PushingHandling(coEventX, false, 1);

				if (coEventX->nx != 0)
					if (isTrigger == false) object->OnCollisionEnter(this, coEventX);
					else object->OnTriggerEnter(this, coEventX);

				//
				// see if after correction on X, is there still a collision on Y ? 
				//
				LPCOLLISIONEVENT colY_other = NULL;

				//
				// check again if there is true collision on X 
				//

				// remove current collision event on Y
				coEventY->isDeleted = true;

				// replace with a new collision event using corrected location 
				coEvents.push_back(SweptAABBEx(coEventY->co));

				// re-filter on Y only
				FilterCollision(coEvents, coEventX, colY_other, /*filterX=*/0, /*filterY=*/1);

				if (colY_other != NULL)
				{
					PushingHandling(colY_other, false, coEventX->t);

					if (isTrigger == false) object->OnCollisionEnter(this, colY_other);
					else object->OnTriggerEnter(this, colY_other);
				}
				else
				{
					pos.y += coEventX->t * dy;
					object->SetPosition(pos);
				}
			}
		}
		else
		{
			if (coEventX != NULL)
			{
				//if (dynamic_cast<CSophia*>(object)) DebugOut(L"X only\n");

				min_t = coEventX->t;

				PushingHandling(coEventX, true, 1);

				if (isTrigger == false) object->OnCollisionEnter(this, coEventX);
				else object->OnTriggerEnter(this, coEventX);
			}
			else
			{
				if (coEventY != NULL)
				{
					//if (dynamic_cast<CSophia*>(object)) DebugOut(L"Y only\n");

					min_t = coEventY->t;

					PushingHandling(coEventY, true, 1);

					if (isTrigger == false) object->OnCollisionEnter(this, coEventY);
					else object->OnTriggerEnter(this, coEventY);
				}
				// both colX & colY are NULL 
				else
				{
					//if (dynamic_cast<CSophia*>(object)) DebugOut(L"both null\n");
					pos.x += dx;
					pos.y += dy;

					object->SetPosition(pos);
				}
			}
		}
	}

	for (UINT i = 0; i < coEvents.size(); i++)
	{
		if (coEvents[i]->t > min_t) continue;

		auto selfCTag = object->GetCTag();
		auto otherCTag = coEvents[i]->obj->GetCTag();

		if ((coEvents[i]->co->IsTrigger() == true) ||
			(CTagUtils::PlayerCTag(selfCTag) && CTagUtils::EnemyCTag(otherCTag)) ||
			(CTagUtils::PlayerCTag(otherCTag) && CTagUtils::EnemyCTag(selfCTag)) ||
			(CTagUtils::PlayerCTag(selfCTag) && otherCTag == CTag::EnemyBullet) ||
			(CTagUtils::PlayerCTag(otherCTag) && selfCTag == CTag::EnemyBullet) ||
			(selfCTag == CTag::BigJason && otherCTag == CTag::HarmPlatform) ||
			(CTagUtils::EnemyCTag(selfCTag) && otherCTag == CTag::HarmPlatform))
		{
			if (isTrigger == false) object->OnCollisionEnter(this, coEvents[i]);
			else object->OnTriggerEnter(this, coEvents[i]);
		}
	}

	DealWithOverlappedCase(coObjects);

	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CCollider2D::PushingHandling(LPCOLLISIONEVENT& coEvent, bool isOnlyAxis, float previousCollisionDuration = 1.0f)
{
	auto pos = object->GetPosition();
	auto velocity = object->GetVelocity();

	if (isOnlyAxis == true)
	{
		if (isTrigger == false)
		{
			pos.x += coEvent->t * dx + coEvent->nx * BLOCK_PUSH_FACTOR;
			pos.y += coEvent->t * dy + coEvent->ny * BLOCK_PUSH_FACTOR;

			if (coEvent->nx != 0) velocity.x = 0;
			if (coEvent->ny != 0) velocity.y = 0;
			object->SetVelocity(velocity);
		}
		else
		{
			pos.x += dx;
			pos.y += dy;
		}
	}
	else
	{
		if (isTrigger == false)
		{
			if (coEvent->nx != 0)
			{
				pos.x += coEvent->t * dx + coEvent->nx * BLOCK_PUSH_FACTOR;
				velocity.x = 0;
			}

			if (coEvent->ny != 0)
			{
				pos.y += coEvent->t * dy + coEvent->ny * BLOCK_PUSH_FACTOR;
				velocity.y = 0;
			}

			object->SetVelocity(velocity);
		}
		else
		{
			if (coEvent->nx != 0) pos.x += dx * previousCollisionDuration;
			if (coEvent->ny != 0) pos.y += dy * previousCollisionDuration;
		}
	}

	object->SetPosition(pos);
}

void CCollider2D::DealWithOverlappedCase(std::vector<CGameObject*>* coObjects)
{
	std::vector<CGameObject*> overlappedObjects;

	for (auto coO : *coObjects)
	{
		if (coO == object) continue;
		if (coO->IsEnabled() == false) continue;
		if (coO->GetColliders().size() == 0) continue;

		auto coOther = coO->GetColliders().at(0);

		auto bbOther = coOther->GetBoundingBox();
		auto bbSelf = GetBoundingBox();

		auto selfCTag = object->GetCTag();
		auto otherCTag = coO->GetCTag();
		if ((CTagUtils::PlayerCTag(selfCTag) && CTagUtils::EnemyCTag(otherCTag)) ||
			(CTagUtils::PlayerCTag(otherCTag) && CTagUtils::EnemyCTag(selfCTag)) ||
			(CTagUtils::PlayerCTag(selfCTag) && otherCTag == CTag::EnemyBullet) ||
			(CTagUtils::PlayerCTag(otherCTag) && selfCTag == CTag::EnemyBullet) ||
			(CTagUtils::EnemyCTag(selfCTag) && otherCTag == CTag::PlayerBullet) ||
			(CTagUtils::EnemyCTag(otherCTag) && selfCTag == CTag::PlayerBullet) ||
			(CTagUtils::EnemyCTag(selfCTag) && otherCTag == CTag::EnemyBullet) ||
			(CTagUtils::EnemyCTag(otherCTag) && selfCTag == CTag::EnemyBullet) ||
			(selfCTag == CTag::MiniPortal && otherCTag == CTag::Jason) ||
			(otherCTag == CTag::MiniPortal && selfCTag == CTag::Jason) ||
			(selfCTag == CTag::BigJason && otherCTag == CTag::HarmPlatform) ||
			(CTagUtils::EnemyCTag(selfCTag) && otherCTag == CTag::HarmPlatform) ||
			(selfCTag == CTag::BossTrigger && otherCTag == CTag::BigJason))
		{
			if (bbSelf.Overlap(bbOther)) object->OnOverlapped(this, coO);
			continue;
		}

		if (bbSelf.Overlap(bbOther) && isTrigger == false && otherCTag == CTag::Platform)
			overlappedObjects.emplace_back(coO);
	}

	for (auto coO : overlappedObjects)
	{
		auto bbOther = coO->GetColliders().at(0)->GetBoundingBox();
		auto bbSelf = GetBoundingBox();

		if (bbSelf.Overlap(bbOther) == false) continue;

		float deltaX = 0, deltaY = 0;

		/*float deltaX_pushRight = 0, deltaX_pushLeft = 0;
		float deltaY_pushTop = 0, deltaY_pushBottom = 0;

		if (bbSelf.left < bbOther.right && bbSelf.left > bbOther.left)
		{
			deltaX_pushRight = bbOther.right - bbSelf.left + BLOCK_PUSH_FACTOR;
		}

		if (bbSelf.right > bbOther.left && bbSelf.right < bbOther.right)
		{
			deltaX_pushLeft = -1 * (bbSelf.right - bbOther.left + BLOCK_PUSH_FACTOR);
		}

		if (deltaX_pushRight != 0 && deltaX_pushLeft != 0)
		{
			if (abs(deltaX_pushRight) < abs(deltaX_pushLeft))
			{
				deltaX = deltaX_pushRight;
			}
			else deltaX = deltaX_pushLeft;
		}
		else
		{
			if (deltaX_pushRight != 0) deltaX = deltaX_pushRight;
			else if (deltaX_pushLeft != 0) deltaX = deltaX_pushLeft;
		}

		if (bbSelf.bottom < bbOther.top && bbSelf.bottom > bbOther.bottom)
		{
			deltaY_pushTop = bbOther.top - bbSelf.bottom + BLOCK_PUSH_FACTOR;
		}

		if (bbSelf.top > bbOther.bottom && bbSelf.top < bbOther.top)
		{
			deltaY_pushBottom = -1 * (bbSelf.top - bbOther.bottom + BLOCK_PUSH_FACTOR);
		}

		if (deltaY_pushTop != 0 && deltaY_pushBottom != 0)
		{
			if (abs(deltaY_pushTop) < abs(deltaY_pushBottom))
			{
				deltaY = deltaY_pushTop;
			}
			else deltaY = deltaY_pushBottom;
		}
		else
		{
			if (deltaY_pushTop != 0) deltaY = deltaY_pushTop;
			else if (deltaY_pushBottom != 0) deltaY = deltaY_pushBottom;
		}*/

		if (bbSelf.left < bbOther.right && bbSelf.left > bbOther.left)
		{
			deltaX += bbOther.right - bbSelf.left + BLOCK_PUSH_FACTOR;
		}

		if (bbSelf.right > bbOther.left && bbSelf.right < bbOther.right)
		{
			deltaX += -1 * (bbSelf.right - bbOther.left + BLOCK_PUSH_FACTOR);
		}

		if (bbSelf.bottom < bbOther.top && bbSelf.bottom > bbOther.bottom)
		{
			deltaY += bbOther.top - bbSelf.bottom + BLOCK_PUSH_FACTOR;
		}

		if (bbSelf.top > bbOther.bottom && bbSelf.top < bbOther.top)
		{
			deltaY += -1 * (bbSelf.top - bbOther.bottom + BLOCK_PUSH_FACTOR);
		}

		if (deltaX != 0 && deltaY != 0)
		{
			if (abs(deltaX) <= abs(deltaY))
			{
				deltaY = 0;
			}
			else deltaX = 0;
		}

		auto newPos = object->GetPosition();
		newPos += Vector2(deltaX, deltaY);
		object->SetPosition(newPos);
	}
}

Rect CCollider2D::GetBoundingBox()
{
	Vector2 posCollider = object->GetPosition() + offset;
	Rect boundingBox;
	boundingBox.left = posCollider.x - boxSize.x / 2;
	boundingBox.top = posCollider.y + boxSize.y / 2;
	boundingBox.right = posCollider.x + boxSize.x / 2;
	boundingBox.bottom = posCollider.y - boxSize.y / 2;
	return boundingBox;
}

void CCollider2D::RenderBoundingBox()
{
	Vector2 posCollider = object->GetPosition() + offset;

	LPDIRECT3DTEXTURE9 bbox = CGame::GetInstance()->GetComponent<CTextures>()->Get("tex-bbox");

	Rect rect;
	rect.left = 0;
	rect.top = 0;
	rect.right = boxSize.x;
	rect.bottom = boxSize.y;

	CGame::GetInstance()->Draw(posCollider, -1, 2, bbox, rect.left, rect.top, rect.right, rect.bottom, 0, D3DCOLOR_ARGB(100, 255, 255, 255));
}
