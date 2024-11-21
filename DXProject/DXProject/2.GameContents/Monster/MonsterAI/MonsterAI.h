#pragma once

class Monster;

enum class MonsterBehavior
{
	Idle,
	Move,
	Attack,
	Attacked,
	Fall,
	Sucked,
};

class MonsterAI
{
public:
	MonsterAI();
	virtual ~MonsterAI();

	virtual void UpdateBehavior(Monster* monster, bool isAttacked = false, bool isSucked = false, bool needStartAttack = false) abstract;
	MonsterBehavior GetPresentBehavior() const { return presentBehavior; }

protected:
	MonsterBehavior presentBehavior;
	vector<MonsterBehavior> behaviorOrder;
	vector<MonsterBehavior>::iterator behaviorOrderIter;

protected:
	void ChangeBehavior(MonsterBehavior behaivor, Monster* Monster);
	void ChangeNextBehavior(Monster* monster);
	void CheckStatusChange(Monster* monster, bool isAttacked, bool isSucked);
};

