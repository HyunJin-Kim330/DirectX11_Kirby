#include "pch.h"
#include "02. GameContents/Player/Ability/SparkAbility.h"
#include "02. GameContents/Weapon/SparkWeapon.h"

SparkAbility::SparkAbility()
{
	animationGatherName = "spark";
	meshName = L"Bottom1.6x1.6";
	attackLoop = true;
	abilityIconTextureName = L"SparkIcon";
	weapon = new SparkWeapon(UserType::Kirby);
	minAttackTime = 0.5f;
}

SparkAbility::~SparkAbility()
{

}

void SparkAbility::Attack()
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
