#include "pch.h"
#include "00. Engine/Component/Animator/Animator.h"
#include "00. Engine/Manager/ResourceManager/Resource/Animation.h"
#include "02. GameContents/Monster/Variety/KingDedede.h"
#include "00. Engine/Component/Collider/RectCollider.h"
#include "02. GameContents/Item/NotMoveStar.h"
#include "02. GameContents/Monster/MonsterAI/FollowPlayerRandomBehaviorAI.h"
#include "02. GameContents/Weapon/KingDededeWeapon.h"
#include "00. Engine/Manager/SceneManager/SceneManager.h"
#include "02. GameContents/Monster/CollisionBehavior/MonsterCollisionBasicBehavior.h"
#include "02. GameContents/Monster/Variety/WaddleDee.h"
#include "02. GameContents/Monster/MonsterAI/WandererAI.h"
#include <random>

KingDedede::KingDedede() : Super(L"Bottom6x6", L"Default")
{
	monsterAI = new FollowPlayerRandomBehaviorAI();
	damage = 20.f;
	jumpPower = 7.5f;
	attackedMaintainTime = 0.5f;
	idleTime = 1.8f;
	flyTime = 6.f;
	spitOutTime = 0.6f;
	initBlood = 100.f;
	initStatus = MonsterStatus::Idle;
	status = initStatus;
	blood = initBlood;
	moveNum = 11;
	suckable = false;
	weapon = new KingDededeWeapon();

	Collider* standardCollider = new RectCollider("BossMonster");
	standardCollider->AddCheckingTag("KirbyBasic");
	standardCollider->AddCheckingTag("RectGrassGround");
	standardCollider->SetCollisionBehavior(new MonsterCollisionBasicBehavior());
	vector<Vec3> vertices(5);
	vertices[0] = Vec3(-0.7f, 1.8f, 0.f);
	vertices[1] = Vec3(0.7f, 1.8f, 0.f);
	vertices[2] = Vec3(0.7f, 0.f, 0.f);
	vertices[3] = Vec3(-0.7f, 0.f, 0.f);
	vertices[4] = Vec3(-0.7f, 1.8f, 0.f);
	standardCollider->SetVertices(vertices);
	AddComponent(standardCollider);

	GetAnimator()->SetAnimationGater("Dedede");

	uint8 animNum = static_cast<uint8>(status);

	if (direction == Direction::RIGHT)
	{
		animNum++;
	}
	GetAnimator()->SetPresentAnimation(animNum);
}

KingDedede::~KingDedede()
{

}

void KingDedede::Move()
{
	switch (moveNum)
	{
	case 1:
		Walk();
		break;
	case 2:
		Fly();
		break;
	case 3:
		DededeJump();
		break;
	default:
		Idle();
		break;
	}
}

void KingDedede::Attack()
{
	if (status == MonsterStatus::Idle)
	{
		Idle();
		return;
	}

	switch (attackNum)
	{
	case 1:
		HamerTakeDownAttack();
		break;
	case 2:
		CallWaddleDeeAttack();
		break;
	case 3:
		HamerJumpTakeDownAttack();
		break;
	default:
		break;
	}
}

void KingDedede::MoveToKirby()
{
	if (status == MonsterStatus::Idle)
	{
		Idle();
		return;
	}

	switch (moveNum)
	{
	case 1:
		Walk();
		break;
	case 2:
		Fly();
		break;
	case 3:
		DededeJump();
		break;
	default:
		Idle();
		break;
	}
}

void KingDedede::Idle()
{
	if (idleTimeCalc < idleTime)
	{
		idleTimeCalc += Time->GetDeltaTime();
	}
	else
	{
		idleTimeCalc = 0.f;
		endOfBehaivor = true;
	}
}

void KingDedede::Walk()
{
	velocity.x = direction == Direction::LEFT ? -2.f : 2.f;

	Vec3 kirbyPosition = GAME->GetSceneManager()->GetKirbyPosition();
	Vec3 myPosition = GetTransform()->GetPosition();

	if (fabsf(kirbyPosition.x - myPosition.x) < 3.f)
	{
		velocity.x = 0.f;
		SetStatus(MonsterStatus::Idle);
	}
}

void KingDedede::Fly()
{
	if (status == MonsterStatus::Fly)
	{
		if (flyTimeCalc < flyTime)
		{
			flyTimeCalc += Time->GetDeltaTime();
		}
		else
		{
			SetStatus(MonsterStatus::SpitOut);
			flyTimeCalc = 0.f;
			velocity.x = 0.f;
			velocity.y = 0.f;
		}

		if (1.2f < flyTimeCalc && flyTimeCalc < 2.2f)
		{
			velocity.y = 3.f;
		}

		if (flyTimeCalc > 2.2f)
		{
			Vec3 kirbyPosition = GAME->GetSceneManager()->GetKirbyPosition();
			Vec3 myPosition = GetTransform()->GetPosition();

			if (kirbyPosition.x < myPosition.x)
			{
				SetDirection(Direction::LEFT, true);
				velocity.x = -1.8f;
			}
			else
			{
				SetDirection(Direction::RIGHT, true);
				velocity.x = 1.8f;
			}
			velocity.y = kirbyPosition.y < myPosition.y ? -1.8f : 1.8f;

			if (grounded)
			{
				velocity.y = 1.8f;
			}
		}
	}
	else if (status == MonsterStatus::SpitOut)
	{
		if (spitOutTimeCalc < spitOutTime)
		{
			spitOutTimeCalc += Time->GetDeltaTime();
		}
		else
		{
			SetStatus(MonsterStatus::Fall);
			velocity.y = -1.5f;
		}

		if (!makedBreath)
		{
			makedBreath = true;
			Vec3 presentPosition = GetTransform()->GetPosition();
			if (direction == Direction::LEFT)
			{
				AddGameObjectToScene(Pool->GetEffectObject(L"dededeSpitOutLeft", presentPosition + Vec3{ -2.5f, 2.f, 0.f }));
			}
			else
			{
				AddGameObjectToScene(Pool->GetEffectObject(L"dededeSpitOutRight", presentPosition + Vec3{ 2.5f, 2.f, 0.f }));
			}
		}
	}
	else if (status == MonsterStatus::Fall)
	{
		if (grounded)
		{
			SetStatus(MonsterStatus::Idle);
		}
	}
	else if (status == MonsterStatus::Idle)
	{
		Idle();
	}
}

void KingDedede::DededeJump()
{
	if (jumpTimeCalc == 0.f)
	{
		// 커비가 멀리 떨어진 경우 앞으로 도약하는 점프
		Vec3 kirbyPosition = GAME->GetSceneManager()->GetKirbyPosition();
		Vec3 myPosition = GetTransform()->GetPosition();

		if (fabsf(kirbyPosition.x - myPosition.x) > 5.f)
		{
			velocity.x = direction == Direction::LEFT ? -3.f : 3.f;
		}
	}

	Jump();

	if (jumpTimeCalc >= 1.f && grounded)
	{
		// 별 추가
		Vec3 presentPosition = GetTransform()->GetPosition();
		AddGameObjectToScene(new NotMoveStar(presentPosition + Vec3{ -3.f, 0.f, 0.f}));
		AddGameObjectToScene(new NotMoveStar(presentPosition + Vec3{ 3.f, 0.f, 0.f }));
		AddGameObjectToScene(Pool->GetEffectObject(L"dededeLandingCloud", presentPosition));

		velocity = Vec3{ 0.f, 0.f, 0.f };
		SetStatus(MonsterStatus::Idle);
	}
}

void KingDedede::HamerTakeDownAttack()
{
	if (attackTimeCalc == 0.f)
	{
		AddGameObjectToScene(weapon);
		Vec3 monsterPosition = GetTransform()->GetPosition();
		queue<KEY_TYPE> keyQueue;
		keyQueue.push(KEY_TYPE::S);
		weapon->Trigger(keyQueue, monsterPosition, direction, this);
	}

	if (attackTimeCalc > attackMaintainTime)
	{
		// 별 추가
		Vec3 presentPosition = GetTransform()->GetPosition();
		if (direction == Direction::LEFT)
		{
			AddGameObjectToScene(new NotMoveStar(presentPosition + Vec3{ -3.f, 0.f, 0.f }));
		}
		else
		{
			AddGameObjectToScene(new NotMoveStar(presentPosition + Vec3{ 3.f, 0.f, 0.f }));

		}
		SetStatus(MonsterStatus::Idle);
	}
	else
	{
		attackTimeCalc += Time->GetDeltaTime();
	}
}

void KingDedede::HamerJumpTakeDownAttack()
{
	if (1.5f <= attackTimeCalc && attackTimeCalc <= 1.6f)
	{
		AddGameObjectToScene(weapon);
		Vec3 monsterPosition = GetTransform()->GetPosition();
		queue<KEY_TYPE> keyQueue;
		keyQueue.push(KEY_TYPE::S);
		weapon->Trigger(keyQueue, monsterPosition, direction, this);
	}

	attackTimeCalc += Time->GetDeltaTime();

	if (1.f <= attackTimeCalc)
	{
		jumpTimeCalc += Time->GetDeltaTime() * 15;
		velocity.y = jumpTimeCalc * (gravity / 2) + jumpPower;
	}

	if (jumpTimeCalc >= 1.f && grounded)
	{
		if (!makedStar)
		{
			// 별 추가
			makedStar = true;

			Vec3 presentPosition = GetTransform()->GetPosition();
			if (direction == Direction::LEFT)
			{
				AddGameObjectToScene(new NotMoveStar(presentPosition + Vec3{ -3.f, 0.3f, 0.f }));
			}
			else
			{
				AddGameObjectToScene(new NotMoveStar(presentPosition + Vec3{ 3.f, 0.3f, 0.f }));
			}
		}

		velocity = Vec3{ 0.f, 0.f, 0.f };

		if (attackMaintainTime <= attackTimeCalc)
		{
			SetStatus(MonsterStatus::Idle);
		}
	}
}

void KingDedede::CallWaddleDeeAttack()
{
	if (attackTimeCalc > attackMaintainTime)
	{
		Vec3 kirbyPosition = GAME->GetSceneManager()->GetKirbyPosition();
		WaddleDee* waddleDee = new WaddleDee(true);
		waddleDee->InitMonster(Vec3{ kirbyPosition.x - 1.f, 6.f, 0.f }, new WandererAI(false));
		AddGameObjectToScene(waddleDee);

		SetStatus(MonsterStatus::Idle);
	}
	else
	{
		attackTimeCalc += Time->GetDeltaTime();
	}
}

Ability* KingDedede::GetAbility()
{
	return nullptr;
}

void KingDedede::Attacked()
{
	if (status != MonsterStatus::Idle && status != MonsterStatus::Fall)
	{
		attackedTimeCalc += Time->GetDeltaTime();

	}

	if (status == MonsterStatus::Idle)
	{
		Idle();
		return;
	}
	if (status == MonsterStatus::Fall)
	{
		if (grounded)
		{
			velocity.y = 0.f;
			endOfBehaivor = true;
		}
	}

	if (attackedTimeCalc >= attackedMaintainTime)
	{
		attackedTimeCalc = 0.0f;

		// 피 양 계산해서 분기 만들기
		if (blood <= 0.f)
		{
			SetStatus(MonsterStatus::Die);
			GetCollider()->Inactivate();
		}
		else
		{
			if (grounded)
			{
				
				SetStatus(MonsterStatus::Idle);

			}
			else
			{
				SetStatus(MonsterStatus::Fall);
				velocity.x = 0.f;
				velocity.y = -3.f;
			}
		}
	}
	else
	{
		velocity.x = 0.f;
	}
}

//void KingDedede::ChooseMove()
//{
//	random_device rd;
//	mt19937 mt(rd());
//	uniform_int_distribution<int> dist(1, 9);
//	int ret = dist(mt);
//
//	if (1 <= ret && ret < 7)
//	{
//		moveNum = 1;
//		SetStatus(MonsterStatus::Walk);
//	}
//	else if (7 <= ret && ret < 8)
//	{
//		moveNum = 2;
//		SetStatus(MonsterStatus::Fly);
//	}
//	else if (8 <= ret && ret < 9)
//	{
//		moveNum = 3;
//		SetStatus(MonsterStatus::Jump);
//	}
//
//	idleTimeCalc = 0.f;
//	walkTimeCalc = 0.f;
//	jumpTimeCalc = 0.f;
//	flyTimeCalc = 0.f;
//	spitOutTimeCalc = 0.f;
//	makedBreath = false;
//}

void KingDedede::ChooseMove()
{

	if (mov == 1)
	{
		moveNum = 1;
		SetStatus(MonsterStatus::Walk);
		mov++;
	}
	else if (mov == 2)
	{
		moveNum = 2;
		SetStatus(MonsterStatus::Fly);
		mov++;
	}
	else if (mov == 3)
	{
		moveNum = 3;
		SetStatus(MonsterStatus::Jump);
		mov = 1;
	}

	idleTimeCalc = 0.f;
	walkTimeCalc = 0.f;
	jumpTimeCalc = 0.f;
	flyTimeCalc = 0.f;
	spitOutTimeCalc = 0.f;
	makedBreath = false;
}

void KingDedede::ChooseAttack()
{
	if (att == 1)
	{
		attackNum = 1;
		attackMaintainTime = 0.72f;
		auto animation = GAME->GetResourceManager()->Get<Animation>(L"Dedede_Attacking1Left");
		GetAnimator()->AddAnimation(4, animation);
		animation = GAME->GetResourceManager()->Get<Animation>(L"Dedede_Attacking1Right");
		GetAnimator()->AddAnimation(5, animation);
		att++;
	}
	else if (att == 2)
	{
		attackNum = 2;
		attackMaintainTime = 2.f;
		auto animation = GAME->GetResourceManager()->Get<Animation>(L"Dedede_CallWaddleDeeAttackLeft");
		GetAnimator()->AddAnimation(4, animation);
		animation = GAME->GetResourceManager()->Get<Animation>(L"Dedede_CallWaddleDeeAttackRight");
		GetAnimator()->AddAnimation(5, animation);
		att++;
	}
	else if (att == 3)
	{
		attackNum = 3;
		attackMaintainTime = 3.32f;
		auto animation = GAME->GetResourceManager()->Get<Animation>(L"Dedede_HamerJumpTakeDownAttackLeft");
		GetAnimator()->AddAnimation(4, animation);
		animation = GAME->GetResourceManager()->Get<Animation>(L"Dedede_HamerJumpTakeDownAttackRight");
		GetAnimator()->AddAnimation(5, animation);
		att = 1;
	}

	GetAnimator()->InitFrameNum();

	idleTimeCalc = 0.f;
	makedStar = false;
}

//void KingDedede::ChooseAttack()
//{
//	random_device rd;
//	mt19937 mt(rd());
//	uniform_int_distribution<int> dist(1, 5);
//	int ret = dist(mt);
//
//	if ( 1 <= ret && ret <= 3)
//	{
//		attackNum = 1;
//		attackMaintainTime = 0.72f;
//		auto animation = GAME->GetResourceManager()->Get<Animation>(L"Dedede_Attacking1Left");
//		GetAnimator()->AddAnimation(4, animation);
//		animation = GAME->GetResourceManager()->Get<Animation>(L"Dedede_Attacking1Right");
//		GetAnimator()->AddAnimation(5, animation);
//	}
//	else if (ret == 4)
//	{
//		attackNum = 2;
//		attackMaintainTime = 2.f;
//		auto animation = GAME->GetResourceManager()->Get<Animation>(L"Dedede_CallWaddleDeeAttackLeft");
//		GetAnimator()->AddAnimation(4, animation);
//		animation = GAME->GetResourceManager()->Get<Animation>(L"Dedede_CallWaddleDeeAttackRight");
//		GetAnimator()->AddAnimation(5, animation);
//	}
//	else if (ret == 5)
//	{
//		attackNum = 3;
//		attackMaintainTime = 3.32f;
//		auto animation = GAME->GetResourceManager()->Get<Animation>(L"Dedede_HamerJumpTakeDownAttackLeft");
//		GetAnimator()->AddAnimation(4, animation);
//		animation = GAME->GetResourceManager()->Get<Animation>(L"Dedede_HamerJumpTakeDownAttackRight");
//		GetAnimator()->AddAnimation(5, animation);
//	}
//
//	GetAnimator()->InitFrameNum();
//
//	idleTimeCalc = 0.f;
//	makedStar = false;
//}

void KingDedede::ChangeBehavior(MonsterBehavior behavior)
{
	endOfBehaivor = false;

	switch (behavior)
	{
	case MonsterBehavior::Move:
		ChooseMove();
		break;
	case MonsterBehavior::Attack:
		ChooseAttack();

		attackTimeCalc = 0.f;
		jumpTimeCalc = 0.f;
		velocity.y = 0.f;
		velocity.x = 0.f;
		SetStatus(MonsterStatus::Attack);
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