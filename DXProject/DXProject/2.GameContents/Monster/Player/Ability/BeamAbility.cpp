#include "pch.h"
#include "BeamAbility.h"
#include "BeamWeapon.h"

BeamAbility::BeamAbility()
{
	animationGatherName = "beam";
	meshName = L"Bottom1.5x1.5";
	attackLoop = false;
	abilityIconTextureName = L"BeamIcon";
	weapon = new BeamWeapon(UserType::Kirby);
	minAttackTime = 0.76f;
}

BeamAbility::~BeamAbility()
{

}

void BeamAbility::Attack()
{
	velocity.x = 0.f;

	if (minAttackTimeCalc < minAttackTime)
	{
		minAttackTimeCalc += Time->GetDeltaTime();
	}
	else
	{
		weapon->StopAttack();
		minAttackTimeCalc = 0.f;
		if (grounded) SetStatus(KirbyStatus::Idle);
		else SetStatus(KirbyStatus::Falling);
	}
}