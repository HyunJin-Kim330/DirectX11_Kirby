#include "pch.h"
#include "Animator.h"
#include "IdleAndAttackAI.h"
#include "Boomer.h"
#include "BoomerangWeapon.h"
#include "RectCollider.h"
#include "MonsterCollisionBasicBehavior.h"

Boomer::Boomer() : Super(L"Bottom1.2x1.2", L"Default")
{
	//monsterAI = new IdleAndAttackAI();
	status = MonsterStatus::Idle;
	initStatus = MonsterStatus::Idle;
	initPosition = Vec3{ 11.6f, 3.8f, 0.f };
	initBlood = 20.f;
	damage = 20.f;
	jumpPower = 5.5f;
	idleTime = 3.5f;
	attackedMaintainTime = 0.5f;

	Collider* standardCollider = new RectCollider("MonsterBasic");
	standardCollider->AddCheckingTag("KirbyBasic");
	standardCollider->AddCheckingTag("RectGrassGround");
	standardCollider->SetCollisionBehavior(new MonsterCollisionBasicBehavior());
	vector<Vec3> vertices(5);
	vertices[0] = Vec3(-0.35f, 0.75f, 0.f);
	vertices[1] = Vec3(0.35f, 0.75f, 0.f);
	vertices[2] = Vec3(0.35f, 0.f, 0.f);
	vertices[3] = Vec3(-0.35f, 0.f, 0.f);
	vertices[4] = Vec3(-0.35f, 0.75f, 0.f);
	standardCollider->SetVertices(vertices);
	AddComponent(standardCollider);

	GetAnimator()->SetAnimationGater("BoomerangMonster");

	uint8 animNum = static_cast<uint8>(status);

	if (direction == Direction::RIGHT)
	{
		animNum++;
	}

	GetAnimator()->SetPresentAnimation(animNum);
}

Boomer::~Boomer()
{

}

void Boomer::Move()
{

}

void Boomer::Attack()
{
	if (attackTimeCalc >= 1.f && !weaponTriggerd)
	{
		weaponTriggerd = true;
		Vec3 position = GetTransform()->GetPosition();
		BoomerangWeapon* weapon = new BoomerangWeapon(UserType::Monster);
		queue<KEY_TYPE> keyQueue;
		keyQueue.push(KEY_TYPE::S);
		weapon->Trigger(keyQueue, position, GetDirectioin(), this);
		AddGameObjectToScene(weapon);
	}

	if (attackNum == 1)
	{
		ShortAttack();
	}
	else if (attackNum == 2)
	{
		LongAttack();
	}
}

void Boomer::DicideAttack()
{
	Vec3 kirbyPosition = GAME->GetSceneManager()->GetKirbyPosition();
	Vec3 monsterPosition = GetTransform()->GetPosition();

	if (kirbyPosition.x < monsterPosition.x) // 커비가 몬스터의 왼쪽
	{
		if (direction == Direction::LEFT)
		{
			attackNum = 1;
		}
		else
		{
			attackNum = 2;
		}
	}
	else									 // 커비가 몬스터의 오른쪽	
	{
		if (direction == Direction::LEFT)
		{
			attackNum = 2;
		}
		else
		{
			attackNum = 1;
		}
	}
}

void Boomer::ShortAttack()
{
	if (attackTimeCalc <= attackMaintainTime)
	{
		attackTimeCalc += Time->GetDeltaTime();
	}
	else
	{
		endOfBehaivor = true;
	}
}

void Boomer::LongAttack()
{
	if (attackTimeCalc <= attackMaintainTime)
	{
		attackTimeCalc += Time->GetDeltaTime();
	}
	else
	{
		Jump();
		if (jumpTimeCalc >= 1.f && grounded)
		{
			velocity.y = 0.f;
			endOfBehaivor = true;
		}
	}
}

Ability* Boomer::GetAbility()
{
	return Pool->GetAbilityObject(L"Boomerang");
}

void Boomer::ChangeBehavior(MonsterBehavior behavior)
{
	endOfBehaivor = false;
	GetAnimator()->InitFrameNum();

	switch (behavior)
	{
	case MonsterBehavior::Idle:
		SetStatus(MonsterStatus::Idle);
		velocity.x = 0.f;
		velocity.y = 0.f;
		idleTimeCalc = 0.f;
		break;
	case MonsterBehavior::Attack:
		SetStatus(MonsterStatus::Attack);
		DicideAttack();
		attackMaintainTime = attackNum == 1 ? 3.2f : 2.4f;
		attackTimeCalc = 0.f;
		weaponTriggerd = false;
		jumpTimeCalc = 0.f;
		velocity.x = 0.f;
		velocity.y = 0.f;
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