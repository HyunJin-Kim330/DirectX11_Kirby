#include "pch.h"
#include "00. Engine/Component/Animator/Animator.h"
#include "02. GameContents/Monster/Variety/SparkMonster.h"
#include "00. Engine/Component/Collider/RectCollider.h"
#include "02. GameContents/Monster/MonsterAI/FollowPlayerAI.h"
#include "02. GameContents/Monster/CollisionBehavior/MonsterCollisionBasicBehavior.h"
#include "02. GameContents/Weapon/SparkWeapon.h"

SparkMonster::SparkMonster() : Super(L"Bottom1x1", L"Default")
{
	damage = 20.f;
	jumpPower = 6.f;
	attackedMaintainTime = 0.5f;
	attackMaintainTime = 3.5f;
	weapon = new SparkWeapon(UserType::Monster);
	initBlood = 20.f;
	

	Collider* standardCollider = new RectCollider("MonsterBasic");
	standardCollider->AddCheckingTag("KirbyBasic");
	standardCollider->AddCheckingTag("RectGrassGround");
	standardCollider->AddCheckingTag("ThinFloor");
	standardCollider->SetCollisionBehavior(new MonsterCollisionBasicBehavior());
	vector<Vec3> vertices(5);
	vertices[0] = Vec3(-0.3f, 0.6f, 0.f);
	vertices[1] = Vec3(0.3f, 0.6f, 0.f);
	vertices[2] = Vec3(0.3f, 0.f, 0.f);
	vertices[3] = Vec3(-0.3f, 0.f, 0.f);
	vertices[4] = Vec3(-0.3f, 0.6f, 0.f);
	standardCollider->SetVertices(vertices);
	AddComponent(standardCollider);

	GetAnimator()->SetAnimationGater("spikeMonster");
}

SparkMonster::~SparkMonster()
{

}

void SparkMonster::Move()
{
	if (jumpTimeCalc == 0.f)
	{
		velocity.x = direction == Direction::LEFT ? -0.5f : 0.5f;
	}

	if (moveNum == 1)
	{
		ShortJump();
	}
	else if (moveNum == 2)
	{
		LongJump();
	}
}

void SparkMonster::MoveToKirby()
{
	if (jumpTimeCalc == 0.f)
	{
		Vec3 kirbyPosition = GAME->GetSceneManager()->GetKirbyPosition();
		Vec3 monsterPosition = GetTransform()->GetPosition();

		float xDistance = kirbyPosition.x - monsterPosition.x;

		if (xDistance < 0.f)
		{
			velocity.x = -0.5f < xDistance ? xDistance : -0.5f;
		}
		else
		{
			velocity.x = 0.5f > xDistance ? xDistance : 0.5f;
		}
	}

	if (moveNum == 1)
	{
		ShortJump();
	}
	else if (moveNum == 2)
	{
		LongJump();
	}
}

void SparkMonster::Attack()
{
	float tempTime = attackTimeCalc * 10;
	tempTime = roundf(tempTime);
	tempTime /= 10;
	if (tempTime == 0.5f)  // 0.5초 뒤 전기공격
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

void SparkMonster::ShortJump()
{
	jumpPower = 4.5f;
	Jump();
	if (jumpTimeCalc >= 1.f && grounded)
	{
		endOfBehaivor = true;
	}
}

void SparkMonster::LongJump()
{
	jumpPower = 7.f;
	Jump();

	if (jumpTimeCalc >= 1.f && grounded)
	{
		endOfBehaivor = true;
	}
}

Ability* SparkMonster::GetAbility()
{
	return Pool->GetAbilityObject(L"Spark");
}

void SparkMonster::ChangeBehavior(MonsterBehavior behavior)
{
	endOfBehaivor = false;
	GetAnimator()->InitFrameNum();

	switch (behavior)
	{
	case MonsterBehavior::Idle:
		SetStatus(MonsterStatus::Idle);
		velocity.x = 0.f;
		velocity.y = 0.f;
		break;
	case MonsterBehavior::Move:
		SetStatus(MonsterStatus::Jump);
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