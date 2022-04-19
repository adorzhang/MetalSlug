#include "RossiExplosion.h"
#include "AnimationManager.h"

void CRossiExplosion::InitAnimations()
{
	auto animation_manager = CGame::GetInstance()->GetSystem<CAnimationManager>();
	AddAnimation(DEFAULT_ANI_NAME, animation_manager->Clone("ani-sophia-explosion"));
	animations.at(DEFAULT_ANI_NAME)->SetIsLooped(false);
}

CRossiExplosion::CRossiExplosion()
{
	InitAnimations();
}

void CRossiExplosion::Render()
{
	auto animation = animations.at(DEFAULT_ANI_NAME);
	if (animation->GetCurrentFrame() == 0)
		animation->Render(transform.position + Vector2(0.0f, -3.0f), 1, layer_index);
	else
	{
		animation->Render(transform.position, 1, layer_index);
	}
}
