#include "pch.h"
#include "Animator.h"
#include "BoomerangAbility.h"
#include "BoomerangWeapon.h"

BoomerangAbility::BoomerangAbility()
{
	animationGatherName = "boomerang";
	meshName = L"Bottom1.3x1.3";
	attackLoop = true;
	abilityIconTextureName = L"CutterIcon";
	minAttackTime = 0.4f;
}

BoomerangAbility::~BoomerangAbility()
{

}

void BoomerangAbility::Attack()
{
	velocity.x = 0.f;

	if (minAttackTimeCalc < minAttackTime)
	{
		minAttackTimeCalc += Time->GetDeltaTime();
	}
	else
	{
		if (pushS == false)
		{
			if (kirby->GetAnimator()->GetFrameNum() < 5)
				return;

			minAttackTimeCalc = 0.f;
			if (grounded) SetStatus(KirbyStatus::Idle);
			else SetStatus(KirbyStatus::Falling);
		}
	}
}

void BoomerangAbility::PushS(Kirby* kirby)
{
	if (Input->GetButtonDown(KEY_TYPE::S))
	{
		pushS = true;
		if (KirbyStatusCheck({ KirbyStatus::FloatingDown, KirbyStatus::FloatingUp }))
		{
			BreathOut();
		}
		else
		{
			if (!KirbyStatusCheck({ KirbyStatus::Attacking }))
			{
				Weapon* boomerang = new BoomerangWeapon(UserType::Kirby);
				kirby->AddGameObjectToScene(boomerang);
				Vec3 kirbyPosition = kirby->GetTransform()->GetPosition();
				queue<KEY_TYPE> keyQueue;
				keyQueue.push(KEY_TYPE::S);

				boomerang->Trigger(keyQueue, kirbyPosition, kirby->GetDirecton(), kirby);

				SetStatus(KirbyStatus::Attacking);
			}
		}
	}

	if (Input->GetButton(KEY_TYPE::S))
	{
		if (kirby->GetKirbyStatus() == KirbyStatus::Attacking)
		{
			if (kirby->GetAnimator()->GetFrameNum() == 3)
			{
				kirby->GetAnimator()->InactivateAnimator();
			}
		}
	}

	if (Input->GetButtonUp(KEY_TYPE::S))
	{
		kirby->GetAnimator()->ActivateAnimator();
		pushS = false;
	}
}