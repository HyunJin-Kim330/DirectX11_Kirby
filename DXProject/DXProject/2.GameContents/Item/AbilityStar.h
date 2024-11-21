#pragma once
#include "GameObject.h"
class Ability;

class AbilityStar : public GameObject
{
public:
	AbilityStar(Ability* _ability, Direction _direction);
	~AbilityStar();

	void Disappear(bool useEffect);
	void AlertLandGround();
	void AlertCollideSideGround(Direction collideDirection);
	void AlertSucked(Vec3 suckedPosition);
	Ability* GetAbility();
	bool GetSucked() { return sucked; }
	void ChangeSucked() { sucked = true; }

private:
	Ability* ability;
	Vec3 velocity = { 0.f, 0.f, 0.f };
	Vec3 suckedPosition;
	float disappearTime = 5.f;
	float disappearTimeCalc = 0.f;
	float jumpPower = 7.f;
	float jumpTimeCalc = 0.f;
	bool sucked = false;

private:
	virtual void UpdateObject() override;
	void Sucked();
	void Jump();

};

