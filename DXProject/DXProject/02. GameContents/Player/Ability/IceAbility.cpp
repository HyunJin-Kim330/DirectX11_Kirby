#include "pch.h"
#include "02. GameContents/Player/Ability/IceAbility.h"
#include "02. GameContents/Weapon/IceWeapon.h"

IceAbility::IceAbility()
{
	animationGatherName = "ice";
	meshName = L"Bottom1.3x1.3";
	attackLoop = true;
	abilityIconTextureName = L"IceIcon";
	minAttackTime = 0.5f;
	weapon = new IceWeapon(UserType::Kirby);
}

IceAbility::~IceAbility()
{

}

void IceAbility::Attack()
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
			weapon->StopAttack();
			minAttackTimeCalc = 0.f;
			if (grounded) SetStatus(KirbyStatus::Idle);
			else SetStatus(KirbyStatus::Falling);
		}
	}
}