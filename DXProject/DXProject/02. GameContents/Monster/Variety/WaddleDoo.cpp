#include "pch.h"
#include "00. Engine/Component/Animator/Animator.h"
#include "00. Engine/Manager/ResourceManager/Resource/Animation.h"
#include "02. GameContents/Player/Ability/BeamAbility.h"
#include "02. GameContents/Weapon/BeamWeapon.h"
#include "00. Engine/Component/Collider/Collider.h"
#include "00. Engine/Component/Collider/RectCollider.h"
#include "02. GameContents/Monster/MonsterAI/NormalAI.h"
#include "02. GameContents/Monster/CollisionBehavior/MonsterCollisionBasicBehavior.h"
#include "02. GameContents/Monster/Variety/WaddleDoo.h"

WaddleDoo::WaddleDoo() : Super(L"Bottom1x1", L"Default")
{
	damage = 20.f;
	walkVelocity = 0.5f;
	jumpPower = 4.5f;
	attackedMaintainTime = 0.5f;
	attackMaintainTime = 1.5f;
	walkTime = 2.5f;
	initBlood = 20.f;
	weapon = new BeamWeapon(UserType::Monster);

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

	GetAnimator()->SetAnimationGater("waddleDoo");
}

WaddleDoo::~WaddleDoo()
{

}

Ability* WaddleDoo::GetAbility()
{
	return Pool->GetAbilityObject(L"Beam");
}

void WaddleDoo::Move()
{
	if (moveNum == 1)
	{
		Walk();
	}
	else if (moveNum == 2)
	{
		WalkAndJump();
	}
}

void WaddleDoo::Attack()
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

void WaddleDoo::Walk()
{
	if (walkTimeCalc < walkTime)
	{
		SetStatus(MonsterStatus::Walk);
		walkTimeCalc += Time->GetDeltaTime();
		velocity.x = direction == Direction::LEFT ? -walkVelocity : walkVelocity;
	}
	else
	{
		endOfBehaivor = true;
	}
}

void WaddleDoo::WalkAndJump()
{
	if (walkTimeCalc < walkTime)
	{
		SetStatus(MonsterStatus::Walk);
		walkTimeCalc += Time->GetDeltaTime();
		velocity.x = direction == Direction::LEFT ? -walkVelocity : walkVelocity;
	}
	else
	{
		Jump();

		if (jumpTimeCalc >= 1.f && grounded)
		{
			endOfBehaivor = true;
		}
	}
}

void WaddleDoo::ChangeBehavior(MonsterBehavior behavior)
{
	endOfBehaivor = false;

	switch (behavior)
	{
	case MonsterBehavior::Move:
		walkTimeCalc = 0.f;
		jumpTimeCalc = 0.f;
		if (moveNum == 1)
		{
			moveNum = 2;
		}
		else if (moveNum == 2)
		{
			velocity.y = 0.f;
			moveNum = 1;
		}
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