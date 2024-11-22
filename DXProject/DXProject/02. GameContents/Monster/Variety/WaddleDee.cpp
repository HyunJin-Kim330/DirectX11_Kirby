#include "pch.h"
#include "00. Engine/Component/Animator/Animator.h"
#include "00. Engine/Manager/ResourceManager/Resource/Animation.h"
#include "00. Engine/Component/Collider/Collider.h"
#include "02. GameContents/Monster/CollisionBehavior/MonsterCollisionBasicBehavior.h"
#include "00. Engine/Component/Collider/RectCollider.h"
#include "00. Engine/Manager/SceneManager/Scene.h"
#include "00. Engine/Manager/SceneManager/SceneManager.h"
#include "02. GameContents/Monster/Variety/WaddleDee.h"
#include "02. GameContents/Monster/MonsterAI/WandererAI.h"

WaddleDee::WaddleDee(bool umbrella) : Super(L"Bottom1x1", L"Default")
{
	haveUmbrella = umbrella;
	suckItem = SuckItem::Star;
	status = MonsterStatus::Walk;
	walkVelocity = 0.9f;
	damage = 10.f;
	attackedMaintainTime = 0.5f;
	initBlood = 10.f;

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

	auto animation = GAME->GetResourceManager()->Get<Animation>(L"waddleDee_WalkingLeft");
	GetAnimator()->AddAnimation(2, animation);
	animation = GAME->GetResourceManager()->Get<Animation>(L"waddleDee_WalkingRight");
	GetAnimator()->AddAnimation(3, animation);
	animation = GAME->GetResourceManager()->Get<Animation>(L"waddleDee_AttackedLeft");
	GetAnimator()->AddAnimation(6, animation);
	animation = GAME->GetResourceManager()->Get<Animation>(L"waddleDee_AttackedRight");
	GetAnimator()->AddAnimation(7, animation);
	animation = GAME->GetResourceManager()->Get<Animation>(L"waddleDee_SuckedLeft");
	GetAnimator()->AddAnimation(8, animation);
	animation = GAME->GetResourceManager()->Get<Animation>(L"waddleDee_SuckedRight");
	GetAnimator()->AddAnimation(9, animation);
	animation = GAME->GetResourceManager()->Get<Animation>(L"waddleDee_UmbrellaDownLeft");
	GetAnimator()->AddAnimation(18, animation);
	animation = GAME->GetResourceManager()->Get<Animation>(L"waddleDee_UmbrellaDownRight");
	GetAnimator()->AddAnimation(19, animation);

	SetStatus(status);		// 현재 상태에 맞게 애니메이션 셋팅
	uint8 animNum = static_cast<uint8>(status);

	if (direction == Direction::RIGHT)
	{
		animNum++;
	}

	GetAnimator()->SetPresentAnimation(animNum);
}

WaddleDee::~WaddleDee()
{

}

void WaddleDee::Move()
{
	if (status == MonsterStatus::UmbrellaDown)
	{
		UmbrellaDown();
	}
	else
	{
		velocity.x = direction == Direction::LEFT ? -walkVelocity : walkVelocity;
	}
}

void WaddleDee::Attack()
{
	// no attack
}

Ability* WaddleDee::GetAbility()
{
	return nullptr;
}

void WaddleDee::ChangeBehavior(MonsterBehavior behavior)
{
	endOfBehaivor = false;

	switch (behavior)
	{
	case MonsterBehavior::Move:
		SetStatus(MonsterStatus::Walk);
		walkTimeCalc = 0.f;
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