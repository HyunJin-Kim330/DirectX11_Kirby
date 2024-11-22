#pragma once
#include "02. GameContents/Player/Ability/Ability.h"

class IceAbility : public Ability
{
public:
	IceAbility();
	~IceAbility();

	virtual void Attack() override;

};

