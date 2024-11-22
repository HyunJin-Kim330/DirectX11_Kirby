#pragma once

enum class UserType
{
	Monster,
	Kirby,
};

class Weapon : public GameObject
{
public:
	Weapon() {}
	virtual ~Weapon() {}

	virtual void Trigger(const queue<KEY_TYPE>& keyQueue, const Vec3 summonerPosition, const Direction direction, GameObject* summoner) abstract;
	void StopAttack(bool deleteObject = false);

	Vec3 GetSummonerPosition() const { return summonerPosition; }
	float GetAttackTime() const { return attackTime; }
	float GetDamage() const { return damage; }

protected:
	UserType user = UserType::Kirby;
	Direction attackDirection;
	Vec3 summonerPosition;
	float attackTime = 0.f;
	float damage = 0.f;

protected:
	virtual void UpdateObject() override;

};

