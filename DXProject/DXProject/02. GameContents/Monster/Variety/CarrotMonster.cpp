#include "pch.h"
#include "00. Engine/Component/Animator/Animator.h"

#include "00. Engine/Manager/ResourceManager/Resource/Animation.h"
#include "02. GameContents/Monster/Variety/CarrotMonster.h"
#include "02. GameContents/Monster/CollisionBehavior/MonsterCollisionBasicBehavior.h"
#include "00. Engine/Component/Collider/RectCollider.h"
#include "00. Engine/Manager/SceneManager/SceneManager.h"

CarrotMonster::CarrotMonster() : Super(L"Bottom1x1", L"Default")
{
	damage = 10.f;
	initBlood = 10.f;
	attackedMaintainTime = 0.5f;
	suckItem = SuckItem::Star;

	Collider* standardCollider = new RectCollider("MonsterBasic");
	standardCollider->AddCheckingTag("KirbyBasic");
	standardCollider->SetCollisionBehavior(new MonsterCollisionBasicBehavior());

	vector<Vec3> vertices(5);
	vertices[0] = Vec3(-0.3f, 0.6f, 0.f);
	vertices[1] = Vec3(0.3f, 0.6f, 0.f);
	vertices[2] = Vec3(0.3f, 0.f, 0.f);
	vertices[3] = Vec3(-0.3f, 0.f, 0.f);
	vertices[4] = Vec3(-0.3f, 0.6f, 0.f);
	standardCollider->SetVertices(vertices);
	AddComponent(standardCollider);

	GetAnimator()->SetAnimationGater("carrotMon");
}

CarrotMonster::~CarrotMonster()
{

}

void CarrotMonster::Move()
{
	if (moveTimeCalc > moveTime)
	{
		moveTimeCalc = 0.f;
		up = up ? false : true;
		moveTime = up ? 0.4f : 0.2f;
	}
	else
	{
		moveTimeCalc += Time->GetDeltaTime();

		if (up)
		{
			velocity.y = 2.f;
		}
		else
		{
			velocity.y = -0.8f;
		}
	}
}

void CarrotMonster::Attack()
{
	Vec3 kirbyPosition = GAME->GetSceneManager()->GetKirbyPosition();
	Vec3 presentPosition = GetTransform()->GetPosition();

	if (kirbyPosition.x < presentPosition.x)
	{
		velocity.x = -2.5f;
	}
	else
	{
		velocity.x = 2.5f;
	}

	if (kirbyPosition.y < presentPosition.y)
	{
		velocity.y = -2.f;
	}
	else
	{
		velocity.y = 2.f;
	}
}

void CarrotMonster::AlertSucked(Direction _suckedDirection, Vec3 kirbyPosition)
{
	SetDirection(_suckedDirection);
	needStartAttack = true;
}

Ability* CarrotMonster::GetAbility()
{
	return nullptr;
}

void CarrotMonster::ChangeBehavior(MonsterBehavior behavior)
{
	endOfBehaivor = false;

	switch (behavior)
	{
	case MonsterBehavior::Move:
		SetStatus(MonsterStatus::Idle);
		velocity.x = 0.f;
		velocity.y = 0.f;
		break;
	case MonsterBehavior::Attack:
		SetStatus(MonsterStatus::Attack);
		needStartAttack = false;
		{
			auto animation = GAME->GetResourceManager()->Get<Animation>(L"carrotMon_AttackedLeft2");
			GetAnimator()->AddAnimation(6, animation);
			animation = GAME->GetResourceManager()->Get<Animation>(L"carrotMon_AttackedRight2");
			GetAnimator()->AddAnimation(7, animation);
		}
		break;
	case MonsterBehavior::Attacked:
		SetStatus(MonsterStatus::Attacked);
		attackedTimeCalc = 0.f;
		break;
	default:
		break;
	}
}