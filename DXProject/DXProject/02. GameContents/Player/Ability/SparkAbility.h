#pragma once
#include "02. GameContents/Player/Ability/Ability.h"

class SparkAbility : public Ability
{
public:
	SparkAbility();
	~SparkAbility();

	virtual void Attack() override;

};

