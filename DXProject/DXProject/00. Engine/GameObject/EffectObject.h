#pragma once
#include "00. Engine/GameObject/GameObject.h"

class EffectObject : public GameObject
{
public:
	EffectObject(wstring _animName, float _lifeTime, wstring _mesh, wstring _material);
	~EffectObject();

private:
	float lifeTime;
	float calcLifeTime = 0.f;

private:
	virtual void UpdateObject() override;

};

