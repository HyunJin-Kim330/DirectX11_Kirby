#include "pch.h"
#include "GreenMonster.h"
#include "RectCollider.h"
#include "Animator.h"
#include "Animation.h"
#include "MonsterCollisionSuckingBehavior.h"
#include "MonsterCollisionBasicBehavior.h"
#include "WandererAI.h"
#include <random>

GreenMonster::GreenMonster() : Super(L"Bottom1.3x1.3", L"Default")
{
	suckItem = SuckItem::Star;
	damage = 10.f;
	attackedMaintainTime = 0.5f;
	walkVelocity = 0.5f;
	walkTime = 1.5f;
	idleTime = 2.1f;
	attackMaintainTime = 1.65f;
	initBlood = 20.f;

	Collider* standardCollider = new RectCollider("MonsterBasic");
	standardCollider->AddCheckingTag("KirbyBasic");
	standardCollider->AddCheckingTag("RectGrassGround");
	standardCollider->SetCollisionBehavior(new MonsterCollisionSuckingBehavior());
	vector<Vec3> vertices(5);
	vertices[0] = Vec3(-0.3f, 0.6f, 0.f);
	vertices[1] = Vec3(0.3f, 0.6f, 0.f);
	vertices[2] = Vec3(0.3f, 0.f, 0.f);
	vertices[3] = Vec3(-0.3f, 0.f, 0.f);
	vertices[4] = Vec3(-0.3f, 0.6f, 0.f);
	standardCollider->SetVertices(vertices);
	AddComponent(standardCollider);

	GetAnimator()->SetAnimationGater("GreenMonster");

	uint8 animNum = static_cast<uint8>(status);

	if (direction == Direction::RIGHT)
	{
		animNum++;
	}

	GetAnimator()->SetPresentAnimation(animNum);
}

GreenMonster::~GreenMonster()
{

}

void GreenMonster::Move()
{
	if (angry)
	{
		Walk();
	}
	else
	{
		if (moveNum == 1)
		{
			Idle();
		}
		else if (moveNum == 2)
		{
			Walk();
		}
	}
}

void GreenMonster::Idle()
{
	if (idleTimeCalc < idleTime)
	{
		idleTimeCalc += Time->GetDeltaTime();
		velocity.x = 0.f;
	}
	else
	{
		idleTimeCalc = 0.f;
		SetStatus(MonsterStatus::Walk);
		moveNum = 2;

		random_device rd;
		mt19937 mt(rd());
		uniform_int_distribution<int> dist(0, 1);
		if (dist(mt) == 0) SetDirection(Direction::LEFT);
		else SetDirection(Direction::RIGHT);
	}
}

void GreenMonster::Walk()
{
	if (angry)
	{
		SetStatus(MonsterStatus::Walk);
		velocity.x = direction == Direction::LEFT ? -walkVelocity : walkVelocity;
	}
	else
	{
		if (walkTimeCalc < walkTime)
		{
			walkTimeCalc += Time->GetDeltaTime();
			velocity.x = direction == Direction::LEFT ? -walkVelocity : walkVelocity;
		}
		else
		{
			walkTimeCalc = 0.f;
			SetStatus(MonsterStatus::Idle);
			moveNum = 1;
		}
	}
}

void GreenMonster::Attack()
{
	if (attackTimeCalc < attackMaintainTime + 0.3f)
	{
		attackTimeCalc += Time->GetDeltaTime();
	}
	else
	{
		endOfBehaivor = true;
	}
}

Ability* GreenMonster::GetAbility()
{
	return nullptr;
}

void GreenMonster::ChangeBehavior(MonsterBehavior behavior)
{
	endOfBehaivor = false;

	switch (behavior)
	{
	case MonsterBehavior::Move:
		SetStatus(MonsterStatus::Idle);
		moveNum = 1;
		walkTimeCalc = 0.f;
		idleTimeCalc = 0.f;
		break;
	case MonsterBehavior::Attack:
		SetStatus(MonsterStatus::Attack);
		needStartAttack = false;
		attackTimeCalc = 0.f;
		velocity.y = 0.f;
		velocity.x = 0.f;
		break;
	case MonsterBehavior::Attacked:
		SetStatus(MonsterStatus::Attacked);
		angry = true;
		walkVelocity = 3.f;
		{
			auto animation = GAME->GetResourceManager()->Get<Animation>(L"GreenMonster_AngryWalkingLeft");
			GetAnimator()->AddAnimation(2, animation);
			animation = GAME->GetResourceManager()->Get<Animation>(L"GreenMonster_AngryWalkingRight");
			GetAnimator()->AddAnimation(3, animation);
			animation = GAME->GetResourceManager()->Get<Animation>(L"GreenMonster_AngryAttackedLeft");
			GetAnimator()->AddAnimation(6, animation);
			animation = GAME->GetResourceManager()->Get<Animation>(L"GreenMonster_AngryAttackedRight");
			GetAnimator()->AddAnimation(7, animation);
			animation = GAME->GetResourceManager()->Get<Animation>(L"GreenMonster_AngrySuckedLeft");
			GetAnimator()->AddAnimation(8, animation);
			animation = GAME->GetResourceManager()->Get<Animation>(L"GreenMonster_AngrSyuckedRight");
			GetAnimator()->AddAnimation(9, animation);
		}
		attackedTimeCalc = 0.f;
		break;
	case MonsterBehavior::Sucked:
		SetStatus(MonsterStatus::Sucked);
		break;
	default:
		break;
	}
}