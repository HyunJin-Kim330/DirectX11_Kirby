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
	void NotCollideAtAllWithGroundBottom();		// � ������ �ظ��� �浹��������
	void NotCollideAtAllWithGroundSide();		// � ������ �� ������ �浹���� ����
	virtual void CollideWithMonster(Direction direction, float damage, AttackType attackType);
	void CollideWithSuckedAbilityMonster(Ability* ability);		// �ɷ�

protected:
	virtual void PushLeftArrow(Kirby* kirby);
	virtual void RightArrowKey(Kirby* kirby);
	virtual void PushDownArrow(Kirby* kirby);
	virtual void PushUpArrow(Kirby* kirby);
	virtual void PushS(Kirby* kirby);
	virtual void PushQ(Kirby* kirby);
	virtual void PushSpace();		// ������ ����

	void CheckStatusWhenUpdate();				// ������Ʈ �� �� Ű �Է��� �ƴ� ���¿����� ������ �ٲ��ֱ� ���� üũ �� �ൿ
	virtual void Attack() abstract;				// ������ ���� ability���� �ٸ��� ������ �߻����� ����
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
	bool attackLoop = false;			// ����Ű�� ������ ������ ������ ���ӵǴ���
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

	bool grounded = true;				// ���� �ִ� ��������
	bool blockRightSide = false;
	bool blockLeftSide = false;
	Vec3 velocity = Vec3{ 0.f, 0.f, 0.f };
	float gravity = 0.f;
	float mass = 3.f;
	float maxWalkVelocity = 3.5f;		// �ȴ� �ӵ�
	float maxDashVelocity = 5.5f;		// �ٴ� �ӵ�
	float maxJumpVelocity = 12.5f;
	float jumpPower = 8.f;
	float jumpTime = 0.f;
	bool wasBendOver = false;
	float minAttackTime = 0.f;			// �ּ� ���������ð�
	float minAttackTimeCalc = 0.f;			// �ּ� ���������ð�
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
	bool KirbyStatusCheck(const vector<KirbyStatus>& statuses);  // Ŀ�� vector ���� ���¿� �ش�Ǵ��� �˻�
	virtual void ChangeAbility() {}
	void SetIdle();

};

