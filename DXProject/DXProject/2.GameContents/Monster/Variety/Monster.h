#pragma once
#include "GameObject.h"
#include "MonsterAI.h"
#include "Scene.h"
#include "SceneManager.h"

class Ability;
class Umbrella;
class Weapon;

enum class MonsterStatus : uint8
{
	Idle = 0,
	Walk = 2,
	Attack = 4,
	Attacked = 6,
	Sucked = 8,
	Fly = 10,
	Jump = 12,
	Fall = 14,
	SpitOut = 16,
	UmbrellaDown = 18,
	Die = 20,
};

enum class AttackType		// 몬스터의 공격 타입
{
	Normal,
	Electric,
	Fire,
	Ice,
	Suck,
	Throw,
};

class Monster : public GameObject
{
public:
	Monster(wstring mesh, wstring material);
	virtual ~Monster();

	void InitMonster(Vec3 _initPosition, MonsterAI* _monsterAI);
	Direction GetDirectioin() const { return direction; }
	MonsterStatus GetStatus() const { return status; }
	MonsterStatus GetBeforeFallingStatus() const { return beforeFallingStatus; }
	SuckItem GetSuckItem() const { return suckItem; }
	Vec3 GetVelocity() const { return velocity; }
	bool GetSuckable() const { return suckable; }
	bool GetGrounded() const { return grounded; }
	float GetDamage() const { return damage; }
	float GetAttackedMaintainTime() const { return attackedMaintainTime; }
	float GetMoveMaintainTime() const { return moveMaintainTime; }
	float GetAttackMaintainTime() const { return attackMaintainTime; }
	float GetJumpTime() const { return jumpTimeCalc; }

	virtual void SetStatus(MonsterStatus _status, bool maintainAnimStatus = false);
	void SetDirection(Direction _direction, bool maintainAnimStatus = false);
	void SetJumpTime(float _jumpTime) { jumpTimeCalc = _jumpTime; }
	void SetMonsterAI(MonsterAI* ai) { monsterAI = ai; }

	virtual void ChangeBehavior(MonsterBehavior behavior) {};

public:
	void AlertAttacked(Direction _attackedDirection, float damage);
	virtual void AlertSucked(Direction _suckedDirection, Vec3 kirbyPosition);
	void AlertBlockSide(Direction _blockDirection);
	void AlertStartAttack();
	void NotCollideAtAllWithGroundBottom();
	void SetGrounded(bool _grounded);
	bool CheckNeedRespone();

	virtual void Idle();
	virtual void Attack() abstract;
	virtual void Attacked();
	virtual void Die();
	virtual void Move() abstract;
	virtual void MoveToKirby() {};
	virtual void Jump();
	virtual void Sucked();
	void UmbrellaDown();
	virtual Ability* GetAbility() abstract;

	bool endOfBehaivor = false;

protected:
	SuckItem suckItem = SuckItem::Ability;
	Direction direction = Direction::LEFT;			// 몬스터가 바라보고 있는 방향
	Direction attackedDirection;						// 피격 방향
	Direction suckedDirection;						// 빨려가는 방향
	Vec3 suckedPosition;
	MonsterAI* monsterAI = nullptr;						// 몬스터 행동 ai
	MonsterStatus status = MonsterStatus::Walk;		// 몬스터 현재 상태
	MonsterStatus initStatus = MonsterStatus::Walk;
	MonsterStatus beforeFallingStatus = MonsterStatus::Walk;
	Weapon* weapon = nullptr;
	Umbrella* umbrella = nullptr;

	bool haveUmbrella = false;
	bool grounded = true;
	bool suckable = true;							// 커비가 빨아들일 수 있는지
	bool attacked = false;
	bool sucked = false;
	bool needStartAttack = false;
	bool die = false;

	float idleTime = 0.f;
	float idleTimeCalc = 0.f;

	float attackedMaintainTime = 0.f;
	float attackedTimeCalc = 0.f;

	float moveMaintainTime = 0.f;

	float flyTime = 0.f;
	float flyTimeCalc = 0.f;

	float walkTime = 0.f;
	float walkTimeCalc = 0.f;

	float attackMaintainTime = 0.f;
	float attackTimeCalc = 0.f;

	float umbrellaTime = 1.5f;
	float umbrellaTimeCalc = 0.f;
	bool umbrellaFront = true;

	float spitOutTime = 0.f;
	float spitOutTimeCalc = 0.f;

	float invincibleTime = 0.f;
	float invincibleTimeCalc = 0.f;

	float damage = 0.f;
	float walkVelocity = 0.f;
	float dashVelocity = 0.f;
	float jumpPower = 0.f;
	float jumpTimeCalc = 0.f;
	float gravity = 0.f;
	float blood = 0.f;
	float initBlood = 0.f;

	Vec3 velocity = Vec3{ 0.f, 0.f, 0.f };

protected:
	virtual void UpdateObject() override;

};

