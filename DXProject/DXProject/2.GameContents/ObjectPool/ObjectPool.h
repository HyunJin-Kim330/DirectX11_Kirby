#pragma once

class Ability;
class EffectObject;

class ObjectPool
{
public:
	ObjectPool();
	~ObjectPool();

	void Init();
	GameObject* GetEffectObject(wstring effectObjectName, Vec3 position);
	Ability* GetAbilityObject(wstring abilityName);

private:
	map<wstring, EffectObject*> effects;
	map<wstring, Ability*> abilities;

private:
	void CreateEffects();
	void CreateAbilities();
};

