#include "Tag.h"

bool CTagUtils::PlayerCTag(CTag tag)
{
	return tag == CTag::Rossi || tag == CTag::Noid ||
		tag == CTag::Mariner || tag == CTag::Player;
}

bool CTagUtils::EnemyCTag(CTag CTag)
{
	return CTag == CTag::Enemy || CTag == CTag::BossZ88;
}
