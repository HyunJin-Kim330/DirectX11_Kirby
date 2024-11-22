#include "pch.h"
#include "ObjectPool.h"
#include "BeamAbility.h"
#include "BoomerangAbility.h"
#include "EffectObject.h"
#include "IceAbility.h"
#include "NormalAbility.h"
#include "SparkAbility.h"

ObjectPool::ObjectPool()
{
	CreateEffects();
	CreateAbilities();
}

ObjectPool::~ObjectPool()
{
	for (auto& pair : effects)
	{
		delete pair.second;
	}

	for (auto& pair : abilities)
	{
		delete pair.second;
	}
}

void ObjectPool::Init()
{

}

GameObject* ObjectPool::GetEffectObject(wstring effectObjectName, Vec3 position)
{
	effects[effectObjectName]->GetTransform()->SetPosition(position);
	return effects[effectObjectName];
}


Ability* ObjectPool::GetAbilityObject(wstring abilityName)
{
	return abilities[abilityName];
}

void ObjectPool::CreateEffects()
{
	tinyxml2::XMLDocument doc;

	XMLError error = doc.LoadFile("effectObject.xml");
	assert(error == XMLError::XML_SUCCESS);

	XmlElement* root = doc.FirstChildElement();

	XmlElement* effect = root->FirstChildElement();
	for (; effect != nullptr; effect = effect->NextSiblingElement())
	{
		string animNameStr = effect->Attribute("AnimName");
		wstring animName = wstring(animNameStr.begin(), animNameStr.end());

		float lifeTime = effect->FloatAttribute("LifeTime");

		string meshStr = effect->Attribute("Mesh");
		wstring mesh = wstring(meshStr.begin(), meshStr.end());

		string materialStr = effect->Attribute("Material");
		wstring material = wstring(materialStr.begin(), materialStr.end());

		EffectObject* effectObject = new EffectObject(animName, lifeTime, mesh, material);

		effects[animName] = effectObject;
	}
}

void ObjectPool::CreateAbilities()
{
	abilities[L"Beam"] = new BeamAbility();
	abilities[L"Boomerang"] = new BoomerangAbility();
	abilities[L"Ice"] = new IceAbility();
	abilities[L"Normal"] = new NormalAbility();
	abilities[L"Spark"] = new SparkAbility();
}