#include "pch.h"
#include "Animator.h"
#include "Animation.h"
#include "IceMonster.h"
#include "RectCollider.h"
#include "MonsterCollisionBasicBehavior.h"
#include "IceWeapon.h"

IceMonster::IceMonster() : Super(L"Bottom1.1x1.1", L"Default")
{
	damage = 20.f;
	walkVelocity = 0.5f;
	attackedMaintainTime = 0.5f;
	attackMaintainTime = 2.f;
	initBlood = 20.f;
	weapon = new IceWeapon(UserType::Monster);
	idleTime = 2.f;

	Collider* standardCollider = new RectCollider("MonsterBasic");
	standardCollider->AddCheckingTag("KirbyBasic");
	standardCollider->AddCheckingTag("RectGrassGround");
	standardCollider->SetCollisionBehavior(new MonsterCollisionBasicBehavior());

	vector<Vec3> vertices(5);
	vertices[0] = Vec3(-0.3f, 0.6f, 0.f);
	vertices[1] = Vec3(0.3f, 0.6f, 0.f);
	vertices[2] = Vec3(0.3f, 0.f, 0.f);
	vertices[3] = Vec3(-0.3f, 0.f, 0.f);
	vertices[4] = Vec3(-0.3f, 0.6f, 0.f);
	standardCollider->SetVertices(vertices);
	AddComponent(standardCollider);

	GetAnimator()->SetAnimationGater("iceMonster");
}

IceMonster::~IceMonster()
{

}

void IceMonster::Move()
{
	if (go)
	{
		velocity.x = direction == Direction::LEFT ? 2.5f : -2.5f;
		goTimeCalc += Time->GetDeltaTime();
		if (goTimeCalc > goTime)
		{
			go = false;
			goTimeCalc = 0.f;
		}
	}
	else
	{
		velocity.x = 0.f;
		stopTimeCalc += Time->GetDeltaTime();
		if (stopTimeCalc > stopTime)
		{
			go = true;
			stopTimeCalc = 0.f;
			endOfBehaivor = true;
		}
	}
}

void IceMonster::MoveToKirby()
{
	if (jumpTimeCalc == 0.f)
	{
		Vec3 kirbyPosition = GAME->GetSceneManager()->GetKirbyPosition();
		Vec3 monsterPosition = GetTransform()->GetPosition();

	}

	if (go)
	{
		velocity.x = direction == Direction::LEFT ? -1.f : 1.f;
		goTimeCalc += Time->GetDeltaTime();
		if (goTimeCalc > goTime)
		{
			go = false;
			goTimeCalc = 0.f;
		}
	}
	else
	{
		velocity.x = 0.f;
		stopTimeCalc += Time->GetDeltaTime();
		if (stopTimeCalc > stopTime)
		{
			go = true;
			stopTimeCalc = 0.f;
			endOfBehaivor = true;
		}
	}
}

void IceMonster::Attack()
{
	if (attackTimeCalc == 0.f)
	{
		AddGameObjectToScene(weapon);
		Vec3 monsterPosition = GetTransform()->GetPosition();
		queue<KEY_TYPE> keyQueue;
		keyQueue.push(KEY_TYPE::S);
		weapon->Trigger(keyQueue, monsterPosition, direction, this);
	}

	if (attackTimeCalc < attackMaintainTime)
	{
		attackTimeCalc += Time->GetDeltaTime();
	}
	else
	{
		endOfBehaivor = true;
		weapon->StopAttack();
	}
}

Ability* IceMonster::GetAbility()
{
	return Pool->GetAbilityObject(L"Ice");
}
void IceMonster::ChangeBehavior(MonsterBehavior behavior)
{
	endOfBehaivor = false;

	switch (behavior)
	{
	case MonsterBehavior::Idle:
		SetStatus(MonsterStatus::Idle);
		velocity.y = 0.f;
		velocity.x = 0.f;
		break;
	case MonsterBehavior::Move:
		SetStatus(MonsterStatus::Walk);
		walkTimeCalc = 0.f;
		jumpTimeCalc = 0.f;
		break;
	case MonsterBehavior::Attack:
		SetStatus(MonsterStatus::Attack);
		attackTimeCalc = 0.f;
		velocity.y = 0.f;
		velocity.x = 0.f;
		break;
	case MonsterBehavior::Attacked:
		SetStatus(MonsterStatus::Attacked);
		attackedTimeCalc = 0.f;
		break;
	case MonsterBehavior::Sucked:
		SetStatus(MonsterStatus::Sucked);
		break;
	default:
		break;
	}
}