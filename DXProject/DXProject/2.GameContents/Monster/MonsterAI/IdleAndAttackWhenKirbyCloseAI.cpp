#include "pch.h"
#include "IdleAndAttackWhenKirbyCloseAI.h"
#include "Monster.h"

IdleAndAttackWhenKirbyCloseAI::IdleAndAttackWhenKirbyCloseAI(float attackDistance)
{
	presentBehavior = MonsterBehavior::Idle;
}

IdleAndAttackWhenKirbyCloseAI::~IdleAndAttackWhenKirbyCloseAI()
{

}

void IdleAndAttackWhenKirbyCloseAI::UpdateBehavior(Monster* monster, bool isAttacked, bool isSucked, bool needStartAttack)
{
	CheckStatusChange(monster, isAttacked, isSucked);

	if (presentBehavior == MonsterBehavior::Idle)
	{
		monster->Idle();
	}
	else if (presentBehavior == MonsterBehavior::Attack)
	{
		monster->Attack();
	}
	else if (presentBehavior == MonsterBehavior::Attacked)
	{
		monster->Attacked();
	}
	else if (presentBehavior == MonsterBehavior::Sucked)
	{
		monster->Sucked();
	}

	{
		if (presentBehavior != MonsterBehavior::Attack && presentBehavior != MonsterBehavior::Sucked && presentBehavior != MonsterBehavior::Attacked)
		{
			Vec3 kirbyPosition = GAME->GetSceneManager()->GetKirbyPosition();
			Vec3 monsterPosition = monster->GetTransform()->GetPosition();

			if (fabsf(kirbyPosition.x - monsterPosition.x) <= attackDistance)
			{
				ChangeBehavior(MonsterBehavior::Attack, monster);
			}
		}
	}

	if (monster->endOfBehaivor)
	{
		ChangeBehavior(MonsterBehavior::Idle, monster);
	}
}