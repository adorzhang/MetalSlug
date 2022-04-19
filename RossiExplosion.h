#pragma once
#include "VisualEffect.h"

class CRossiExplosion : public CVisualEffect
{
protected:
	void InitAnimations();
public:
	CRossiExplosion();
	void Render() override;
};

