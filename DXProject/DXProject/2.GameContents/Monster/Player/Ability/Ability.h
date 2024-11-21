#pragma once
#include "Kirby.h"

class Monster;
class Weapon;

class Ability
{
public:
	Ability();
	virtual ~Ability();
	
public:
	void Update(Kirby* kirby);

	Vec3 GetVelocity() const { return velocity; }
	bool GetGrounded() const { return grounded; }
	string GetAnimationGatherName() const{ return animationGatherName; }
	wstring GetMeshName() const { return meshName; }
	wstring GetAbilityIconTextureName () const { return abilityIconTextureName; }

	void SetKirby(Kirby* _kirby) { kirby = _kirby; }
	void SetGrounded(bool _grounded);

	void ChangeKirbyAnimationGather(string gatherName);
	void ChangeKirbyMesh(wstring meshName);
	void BlockLeft();
	void BlockRight();
	void BlockTop();
	void NotCollideAtAllWithGroundBottom();		// 어떤 땅과도 밑면이 충돌하지않음
	void NotCollideAtAllWithGroundSide();		// 어떤 땅과도 양 옆면이 충돌하지 않음
	virtual void CollideWithMonster(Direction direction, float damage, AttackType attackType);
	void CollideWithSuckedAbilityMonster(Ability* ability);		// 능력

protected:
	virtual void PushLeftArrow(Kirby* kirby);
	virtual void RightArrowKey(Kirby* kirby);
	virtual void PushDownArrow(Kirby* kirby);
	virtual void PushUpArrow(Kirby* kirby);
	virtual void PushS(Kirby* kirby);
	virtual void PushQ(Kirby* kirby);
	virtual void PushSpace();		// 점프로 구현

	void CheckStatusWhenUpdate();				// 업데이트 할 때 키 입력이 아닌 상태에의해 뭔가를 바꿔주기 위해 체크 및 행동
	virtual void Attack() abstract;				// 공격은 정말 ability별로 다르기 때문에 추상으로 정의
	virtual void Attacked();
	void Die();
	void AttackedElecricOrFire();
	void ShockedLand();
	void BreathOut();
	void SetStatus(KirbyStatus status);
	void ChangeVelocityOnTheRampGround(Direction goingDirection);
	void test(Direction direction);

protected:
	bool pushS = false;
	bool attackLoop = false;			// 공격키를 누르고 있으면 공격이 지속되는지
	int leftArrow = 0;
	int rightArrow = 0;
	float potentialOfDashLeft = 0.5f;
	float potentialOfDashRight = 0.5f;
	float maintainSpace = 0.f;
	float maintainUp = 0.f;
	float maintainSpitStatus = 0.f;
	float maintainAttackedStatus = 0.f;

protected:
	Kirby* kirby;
	string animationGatherName = "normal";
	wstring meshName = L"Bottom1x1";
	wstring abilityIconTextureName = L"NormalIcon";
	Ability* tempAbility = nullptr;

	bool grounded = true;				// 땅에 있는 상태인지
	bool blockRightSide = false;
	bool blockLeftSide = false;
	Vec3 velocity = Vec3{ 0.f, 0.f, 0.f };
	float gravity = 0.f;
	float mass = 3.f;
	float maxWalkVelocity = 3.5f;		// 걷는 속도
	float maxDashVelocity = 5.5f;		// 뛰는 속도
	float maxJumpVelocity = 12.5f;
	float jumpPower = 8.f;
	float jumpTime = 0.f;
	bool wasBendOver = false;
	float minAttackTime = 0.f;			// 최소 공격유지시간
	float minAttackTimeCalc = 0.f;			// 최소 공격유지시간
	float bendoverTime = 0.12f;
	float bendoverTimeCalc = 0.f;
	float handsUpTime = 0.5f;
	float handsUpTimeCalc = 0.f;
	float landTimeCalc = 0.f;
	float dieTimeCalc = 0.f;
	float digestingTime = 0.f;
	Weapon* weapon = nullptr;
	KirbyStatus beforeJumpStatus = KirbyStatus::Idle;
	KirbyStatus beforeFallStatus = KirbyStatus::Idle;

protected:
	bool KirbyStatusCheck(const vector<KirbyStatus>& statuses);  // 커비가 vector 안의 상태에 해당되는지 검사
	virtual void ChangeAbility() {}
	void SetIdle();

};

