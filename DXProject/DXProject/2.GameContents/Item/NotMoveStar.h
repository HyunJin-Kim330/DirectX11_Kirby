#pragma once
#include "GameObject.h"
class NotMoveStar : public GameObject
{
public:
	NotMoveStar(Vec3 position);
	~NotMoveStar();

	virtual void UpdateObject() override;
	bool GetSucked() { return sucked; }
	void Disappear();
	void AlertSucked(Vec3 _suckedPosition);

private:
	bool sucked = false;
	Vec3 suckedPosition = Vec3{ 0.f, 0.f, 0.f };
	Vec3 velocity = Vec3{ 0.f, 0.f, 0.f };

	float lifeTime = 3.f;
	float lifeTimeCalc = 0.f;
};

