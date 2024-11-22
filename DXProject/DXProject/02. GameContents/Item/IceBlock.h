#pragma once
#include "00. Engine/GameObject/GameObject.h"

class IceBlock : public GameObject
{
public:
	IceBlock(Vec3 position);
	~IceBlock();

	virtual void UpdateObject() override;
	void Disappear();

private:
	Vec3 velocity = Vec3{ 0.f, 0.f, 0.f };
	float lifeTime = 3.f;
	float lifeTimeCalc = 0.f;

};

