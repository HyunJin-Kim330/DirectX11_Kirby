#pragma once
#include "02. GameContents/Monster/Variety/Monster.h"

class CarrotMonster : public Monster
{
	using Super = Monster;
public:
	CarrotMonster();
	~CarrotMonster();

public:
	virtual void Move() override;
	virtual void Attack() override;

	virtual Ability* GetAbility() override;
	virtual void ChangeBehavior(MonsterBehavior behavior);
	virtual void AlertSucked(Direction _suckedDirection, Vec3 kirbyPosition);

private:
	//bool angry = false;
	bool up = true;
	float moveTime = 0.2f;
	float moveTimeCalc = 0.f;

};
