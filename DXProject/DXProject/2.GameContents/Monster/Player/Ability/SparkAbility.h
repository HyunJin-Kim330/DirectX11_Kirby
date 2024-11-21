#pragma once
#include "Ability.h"

class SparkAbility : public Ability
{
public:
	SparkAbility();
	~SparkAbility();

	virtual void Attack() override;

};

