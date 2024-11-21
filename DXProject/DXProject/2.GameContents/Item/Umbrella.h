#pragma once
#include "GameObject.h"

class Umbrella : public GameObject
{
public:
	Umbrella();
	~Umbrella();

	virtual void UpdateObject() override;

	void ReleaseFromMonster();
	void SetSuckedTrue(Vec3 suckedPosition);

private:
	bool atatchMonster = true;
	bool sucked = false;
	Vec3 velocity = { 0.f, 0.f, 0.f };

private:
	void Disappear();
};

