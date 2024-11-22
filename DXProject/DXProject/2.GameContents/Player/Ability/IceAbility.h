#pragma once
#include "Ability.h"

class IceAbility : public Ability
{
public:
	IceAbility();
	~IceAbility();

	virtual void Attack() override;

};

