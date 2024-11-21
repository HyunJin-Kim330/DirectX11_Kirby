#include "pch.h"
#include "Animator.h"
#include "NormalAbility.h"
#include "NormalWeapon.h"
#include "Scene.h"
#include "SceneManager.h"
#include "StarWeapon.h"
#include "Weapon.h"


NormalAbility::NormalAbility()
{
	weapon = new NormalWeapon();
	animationGatherName = "normal";
	meshName = L"Bottom1x1";
	abilityIconTextureName = L"NormalIcon";
}

NormalAbility::~NormalAbility()
{

}

void NormalAbility::PushS(Kirby* kirby)
{
	if (KirbyStatusCheck({ KirbyStatus::UpLadder, KirbyStatus::DownLadder })) return;

	if (Input->GetButtonDown(KEY_TYPE::S))
	{
		velocity.x = 0.f;
		if (KirbyStatusCheck({ KirbyStatus::FloatingDown , KirbyStatus::FloatingUp }))
		{
			BreathOut();
			return;
		}
		else if (kirby->GetSucked())
		{
			StarSpitOut();
		}
		else
		{
			kirby->SetKirbyStatus(KirbyStatus::Sucking);
			SuckingAttack(kirby);
		}
	} 

	if (Input->GetButtonUp(KEY_TYPE::S))
	{
		// 아무 몬스터랑도 안부딪히고있으면 공격 없앰
		weapon->StopAttack();
		if (grounded)
		{
			SetStatus(KirbyStatus::Idle);
		}
		else
		{
			SetStatus(KirbyStatus::Falling);
		}
	}
}

void NormalAbility::PushDownArrow(Kirby* kirby)
{
	if (Input->GetButtonDown(KEY_TYPE::DOWN))
	{
		if (KirbyStatusCheck({ KirbyStatus::FloatingUp, KirbyStatus::FloatingDown })) return;

		if (grounded)
		{
			if (kirby->GetSucked())
			{ 
				SetStatus(KirbyStatus::Digesting);

				if (kirby->GetCollideWithAbilityMonster())
				{
					GAME->GetSceneManager()->GetActiveScene()->AddGameObject(Pool->GetEffectObject(L"changeAbility", kirby->GetMidstPosition()));
				}
			}
			else
			{
				if (kirby->GetOnTheLadderTop())
				{
					SetStatus(KirbyStatus::DownLadder);
					velocity.y = -4.5f;
				}
				else
				{
					kirby->SetKirbyStatus(KirbyStatus::Foldding);
					velocity.x = 0.f;
				}
			}
		}
		else
		{
			if (kirby->GetFrontLadder())
			{
				SetStatus(KirbyStatus::DownLadder);
				velocity.y = -4.5f;
				kirby->GetAnimator()->ActivateAnimator();
			}
		}
	}

	if (Input->GetButton(KEY_TYPE::DOWN))
	{
		if (KirbyStatusCheck({ KirbyStatus::DownLadder }))
		{
			if (grounded && !kirby->GetOnTheLadderTop())
			{
				SetIdle();
				kirby->GetAnimator()->ActivateAnimator();
			}
		}
	}

	if (Input->GetButtonUp(KEY_TYPE::DOWN))
	{
		if (grounded && KirbyStatusCheck({ KirbyStatus::Foldding }))
		{
			SetIdle();
		}
		else if (KirbyStatusCheck({ KirbyStatus::DownLadder }))
		{
			velocity.y = 0.f;
			kirby->GetAnimator()->InactivateAnimator();
		}
	}
}

void NormalAbility::Attack()
{
	// 커비 상태 바꾸기

}

void NormalAbility::ChangeAbility()
{
	if (tempAbility != nullptr)
	{
		kirby->SetAbility(tempAbility);
		tempAbility->SetKirby(kirby);

		ChangeKirbyAnimationGather(tempAbility->GetAnimationGatherName());
		ChangeKirbyMesh(tempAbility->GetMeshName());
		SetStatus(KirbyStatus::HandsUp);
	}
}

void NormalAbility::SuckingAttack(Kirby* kirby)
{
	//weapon = new NormalWeapon();
	kirby->AddGameObjectToScene(weapon);
	
	Vec3 kirbyPosition = kirby->GetTransform()->GetPosition();

	queue<KEY_TYPE> keyQueue;
	keyQueue.push(KEY_TYPE::S);
	weapon->Trigger(keyQueue, kirbyPosition, kirby->GetDirecton(), kirby);

}

void NormalAbility::StarSpitOut()
{
	SetStatus(KirbyStatus::SpitOut);
	kirby->SetSucked(false);

	Vec3 kirbyPosition = kirby->GetTransform()->GetPosition();
	queue<KEY_TYPE> keyQueue;
	keyQueue.push(KEY_TYPE::S);

	StarWeapon* starWeapon = new StarWeapon();
	starWeapon->Trigger(keyQueue, kirbyPosition, kirby->GetDirecton(), kirby);
	kirby->AddGameObjectToScene(starWeapon);
}

void NormalAbility::CollideWithMonster(Direction direction, float damage, AttackType attackType)
{
	if (kirby->isInvincible()) return;

	kirby->DecreaseBlood(damage);

	if (kirby->GetBlood() <= 0.f)
	{
		SetStatus(KirbyStatus::Die);
	}
	else
	{
		kirby->SetDirection(direction);
		switch (attackType)
		{
		case AttackType::Normal:
			SetStatus(KirbyStatus::Attacked);
			break;
		case AttackType::Electric:
			SetStatus(KirbyStatus::ElectricShock);
			break;
		default:
			SetStatus(KirbyStatus::Attacked);
			break;
		}
	}
}