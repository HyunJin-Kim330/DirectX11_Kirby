#pragma once
#include "Collider.h"
#include "Monster.h"

enum class KirbyStatus : uint8
{
	Idle = 0,
	Walking = 2,
	Dash = 4,
	IdleLeftTilt = 6,
	IdleRightTilt = 8,
	FloatingUp = 10,
	FloatingDown = 12,
	Sucking = 14,
	SpitOut = 16,
	Jumping = 18,
	Falling = 20,
	Attacking = 22,
	Attacked = 24,
	Foldding = 26,
	SuckedIdle = 28,
	SuckedWalking = 30,
	SuckedDash = 32,
	SuckedJumping = 34,
	SuckedFalling = 36,
	SuckedAttacked = 38,
	Digesting = 40,			// ��ȭ
	HandsUp = 42,
	SuckedIdleLeftTilt = 44,
	SuckedIdleRightTilt = 46,
	Die = 48,
	BendOver = 50,
	ElectricShock = 52,
	SuckedElectricShock = 54,
	ShockedLand = 56,
	UpLadder = 58,
	DownLadder = 60,
	EnterDoor = 62,
};

class Kirby : public GameObject
{
public:
	Kirby(Ability* _ability);
	~Kirby();

public:
	Ability* GetAbility() { return ability; }
	Direction GetDirecton() const { return direction; }
	KirbyStatus GetKirbyStatus() const { return status; }
	Vec3 GetVelocity() const;
	Vec3 GetTiltLeftDirection() { return tiltLeftDirection; }
	Vec3 GetTiltRightDirection() { return tiltRightDirection; }

	bool isInvincible() { return invincible; }
	bool GetSucked() const { return sucked; }
	bool GetFrontDoor() const { return frontDoor; }
	bool GetFrontLadder() const { return frontLadder; }
	bool GetOnTheLadderTop() const { return onTheLadderTop; }
	bool GetCollideWithAbilityMonster() { return collideWithAbilityMonster; }
	bool GetCollideWithLeftRampGrass() { return collideWithLeftRampGrass; }
	bool GetCollideWithRightRampGrass() { return collideWithRightRampGrass; }
	bool GetInsideMonsterMouth() { return insideMonsterMouth; }
	float GetBlood() const { return blood; }
	float GetInsideMonsterMouthTime() const { return insideMonsterMouthTime; }
	wstring GetAbilityInfoTexture() const;
	int GetLife() const { return life; }

	void SetKirbyStatus(KirbyStatus _status);
	void SetAbility(Ability* _ability) { ability = _ability; }  // ���Ϳ��� � �����Ƽ�� �ٲ�ߵǴ��� �����Ƽ ��ü ��������
	void SetSucked(bool _val) { sucked = _val; }
	void SetDirection(Direction _direction);
	void SetFrontDoor(bool val) { frontDoor = val; }
	void SetFrontLadder(bool val) { frontLadder = val; }
	void SetOnTheLedderTop(bool val) { onTheLadderTop = val; }
	void SetGrounded(bool _gronded);
	void SetCollideWithAbilityMonster(bool _val) { collideWithAbilityMonster = _val; }
	void SetCollideWithBottomGrass(bool val) { collideWithBottomGrass = val; }
	void SetCollideWithLeftRampGrass(bool val) { collideWithLeftRampGrass = val; }
	void SetCollideWithRightRampGrass(bool val) { collideWithRightRampGrass = val; }
	void SetTiltLeftDirection(Vec3 val) { tiltLeftDirection = val; }
	void SetTiltRightDirection(Vec3 val) { tiltRightDirection = val; }

	void InsideMonsterMouth(float attackTime);
	void OutsideMonsterMouth();
	float DecreaseBlood(float amount);
	float IncreaseBlood(float amount);
	void PlusLife() { life++; }
	void MinusLife() { life--; }

	void AlertBlockSide(CollisionDirection direction);
	void AlertBlockTop();
	void NotCollideAtAllWithGroundBottom();
	void NotCollideAtAllWithGroundSide();
	void AlertCollideWithMonster(Direction direction, float damage, AttackType attackType);		// ���Ϳ� �Ϲ������� �浹���� ��
	void AlertCollideWithSuckedAbilityMonster(Ability* ability);			// ���Ƶ��ӻ����� ���Ϳ� �浹���� ��(���Ͱ� Ư�� �ɷ��� ���� ���)
	void AlertCollideWithSuckedStarMonster();								// ���Ƶ��ӻ����� ���Ϳ� �浹���� ��(�ɷ¾��� ����̶� �浹)
	void AlertChangeScene(bool revive = false);

	// ��ǥ����
	void SetPositionWithPlus(Vec3 amount);

private:
	Ability* ability;  // Ŀ�� �ɷ�
	KirbyStatus status = KirbyStatus::Idle;
	Direction direction = Direction::RIGHT;

	int life = 2;
	float blood = 100.f;
	float maxSpeed;
	float insideMonsterMouthTime = 0.f;
	float insideMonsterMouthTimeCalc = 0.f;
	float invincibleTime = 3.f;
	float invincibleTimeCalc = 0.f;

	bool frontDoor = false;
	bool frontLadder = false;
	bool onTheLadderTop = false;
	bool invincible = false;
	bool sucked = false;
	bool insideMonsterMouth = false;
	bool collideWithAbilityMonster = false;
	bool collideWithBottomGrass = false;
	bool collideWithLeftRampGrass = false;
	bool collideWithRightRampGrass = false;
	Vec3 tiltLeftDirection = { 0.f,0.f, 0.f };
	Vec3 tiltRightDirection = { 0.f,0.f, 0.f };

private:
	virtual void UpdateObject() override;

};
