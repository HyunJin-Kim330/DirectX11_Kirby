#pragma once
#include  "Ability.h"

class BeamAbility : public Ability
{
public:
	BeamAbility();
	~BeamAbility();

	virtual void Attack() override;

};

