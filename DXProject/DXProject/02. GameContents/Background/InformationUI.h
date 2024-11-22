#pragma once
#include "00. Engine/GameObject/GameObject.h"

class EmptyObject;
class Kirby;

class InformationUI : public GameObject
{
public:
	InformationUI();
	~InformationUI();

	virtual void UpdateObject() override;
	void UpdateInfo(const Kirby* kirby, float monsterRemainBloodRatio = -1.f);

private:
	EmptyObject* abilityInfo = nullptr;
	EmptyObject* lifeInfo = nullptr;
	EmptyObject* kirbyBloodInfo = nullptr;
	EmptyObject* kirbyBloodBorder = nullptr;
	EmptyObject* monsterBloodInfo = nullptr;
	EmptyObject* monsterBloodBorder = nullptr;

	wstring abilityInfoTexture = L"NormalIcon";
	wstring kirbyLifeInfoTexture = L"life2";

	float monsterBloodDrawTime = 2.f;
	float monsterBloodDrawTimeCalc = 2.f;
	
	float monsterRemainBloodRatio = 1.f;

private:
	void CreateAbilityInfo();
	void CreateLifeInfo();
	void CreateKirbyBlood();
	void CreateMonsterBlood();

	void FollowCamera();
};

