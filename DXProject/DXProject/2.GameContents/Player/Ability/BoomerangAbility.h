#pragma once
#include "Ability.h"

class BoomerangAbility : public Ability
{
public:
	BoomerangAbility();
	~BoomerangAbility();

	virtual void Attack() override;

protected:
	virtual void PushS(Kirby* kirby);

private:
	float attackFinishTime = 0.08f;
	float attackFinishTimeCalc = 0.f;

};

