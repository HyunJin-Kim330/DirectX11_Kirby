#include "pch.h"
#include "NormalAI.h"
#include "Scene.h"
#include "SceneManager.h"

NormalAI::NormalAI()
{
	behaviorOrder.push_back(MonsterBehavior::Move);
	behaviorOrder.push_back(MonsterBehavior::Attack);
	behaviorOrderIter = behaviorOrder.begin();

	presentBehavior = *behaviorOrderIter;
}

void NormalAI::UpdateBehavior(Monster* monster, bool isAttacked, bool isSucked, bool needStartAttack)
{
	CheckStatusChange(monster, isAttacked, isSucked);

	if (presentBehavior == MonsterBehavior::Move)
	{
		monster->Move();
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

	if (monster->endOfBehaivor)
	{
		ChangeNextBehavior(monster);
	}
}

//void NormalAI::UpdateBehavior(Monster* monster, bool isAttacked, bool isSucked)
//{
//	if (isAttacked)
//	{
//		monster->SetStatus(MonsterStatus::Attacked);
//	}
//
//	if (isSucked)
//	{
//		monster->SetStatus(MonsterStatus::Sucked);
//	}
//
//	if (monster->GetStatus() == MonsterStatus::Attacked)
//	{
//		monster->Attacked();
//
//		attackedMaintainTime += Time->GetDeltaTime();
//
//		if (attackedMaintainTime > monster->GetAttackedMaintainTime())
//		{
//			attackedMaintainTime = 0.f;
//
//			// 피 양 계산해서 분기 만들기
//			GAME->GetSceneManager()->GetActiveScene()->RemoveGameObject(monster);
//			GAME->GetSceneManager()->GetActiveScene()->AddGameObject(Pool->GetEffectObject(L"monsterExplosionBasic", monster->GetMidstPosition()));
//		}
//	}
//
//	if (monster->GetStatus() == MonsterStatus::Walk)
//	{
//		monster->Move();
//
//		moveMaintainTime += Time->GetDeltaTime();
//
//		if (moveMaintainTime > monster->GetMoveMaintainTime())
//		{
//			moveMaintainTime = 0.f;
//			ChangeStatus(monster);
//		}
//	}
//
//	if (monster->GetStatus() == MonsterStatus::Jump || 
//		(monster->GetBeforeFallingStatus() == MonsterStatus::Jump && monster->GetStatus() == MonsterStatus::Fall))
//	{
//		monster->Jump();
//
//		if (monster->GetJumpTime() >= 1.f && monster->GetGrounded())
//		{
//			monster->SetJumpTime(0.f);
//			ChangeStatus(monster);
//		}
//	}
//
//	if (monster->GetStatus() == MonsterStatus::Attack)
//	{
//		monster->Attack();
//
//		attackMaintainTime += Time->GetDeltaTime();
//
//		if (attackMaintainTime > monster->GetAttackMaintainTime())
//		{
//			attackMaintainTime = 0.f;
//			ChangeStatus(monster);
//		}
//	}
//
//	if (monster->GetStatus() == MonsterStatus::Sucked)
//	{
//		monster->Sucked();
//	}
//}

//void NormalAI::ChangeStatus(Monster* monster)
//{
//	statusOrderIter++;
//	if (statusOrderIter == statusOrder.end())
//	{
//		statusOrderIter = statusOrder.begin();
//	}
//
//	monster->SetStatus(*statusOrderIter);
//	attackMaintainTime = 0.f;
//	attackedMaintainTime = 0.f;
//	moveMaintainTime = 0.f;
//}