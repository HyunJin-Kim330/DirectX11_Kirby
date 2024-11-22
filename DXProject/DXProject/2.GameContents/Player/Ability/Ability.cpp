#include "pch.h"
#include "Ability.h"
#include "AbilityStar.h"
#include "Animator.h"
#include "InputManager.h"
#include "BreathWeapon.h"
#include "MeshRenderer.h"
#include "Monster.h"
#include "Scene.h"
#include "SceneManager.h"

Ability::Ability()
{
	
}

Ability::~Ability()
{

}

void Ability::CheckStatusWhenUpdate()
{
	if (kirby->GetKirbyStatus() == KirbyStatus::Digesting)
	{
		if (kirby->GetCollideWithAbilityMonster())
		{
			if (digestingTime < 1.f)
			{
				digestingTime += Time->GetDeltaTime();
			}
			else
			{
				digestingTime = 0.f;
				kirby->SetSucked(false);
				kirby->SetCollideWithAbilityMonster(false);
				ChangeAbility();
			}
		}
		else
		{
			if (digestingTime < 0.32f)
			{
				digestingTime += Time->GetDeltaTime();
			}
			else
			{
				digestingTime = 0.f;
				kirby->SetSucked(false);
				SetStatus(KirbyStatus::Idle);
			}
		}
		return;
	}

	if (KirbyStatusCheck({ KirbyStatus::Die }))
	{
		Die();
		Vec3 v = Vec3{ velocity.x, gravity * mass + velocity.y, velocity.z };
		kirby->SetPositionWithPlus(v * Time->GetDeltaTime());
		return;
	}

	if (KirbyStatusCheck({ KirbyStatus::Jumping, KirbyStatus::SuckedJumping, KirbyStatus::BendOver }) || (KirbyStatusCheck({ KirbyStatus::Falling, KirbyStatus::SuckedFalling }) && beforeFallStatus == KirbyStatus::BendOver))
	{
		float customSpeed = 1.5f;
		jumpTime += Time->GetDeltaTime() * 40 * customSpeed;
		velocity.y = jumpTime * (gravity / 2) + jumpPower - gravity * mass;
		velocity.y *= customSpeed;

		if (KirbyStatusCheck({ KirbyStatus::Falling, KirbyStatus::SuckedFalling }) && (beforeFallStatus == KirbyStatus::Jumping || beforeFallStatus == KirbyStatus::SuckedJumping))
		{
			velocity.y = -velocity.y;
		}
	}

	if (KirbyStatusCheck({ KirbyStatus::Jumping }))
	{
		if (!wasBendOver && velocity.y <= 0.f && jumpTime >= 20.f)
		{
			wasBendOver = true;
			SetStatus(KirbyStatus::BendOver);
		}
	}

	if (KirbyStatusCheck({ KirbyStatus::BendOver }))
	{
		if (bendoverTimeCalc < bendoverTime)
		{
			bendoverTimeCalc += Time->GetDeltaTime();
		}
		else
		{
			bendoverTimeCalc = 0.f;
			SetStatus(KirbyStatus::Falling);
		}
	}

	if (KirbyStatusCheck({ KirbyStatus::HandsUp }))
	{
		if (handsUpTimeCalc < handsUpTime)
		{
			velocity.x = 0.f;
			handsUpTimeCalc += Time->GetDeltaTime();
		}
		else
		{
			handsUpTimeCalc = 0.f;
			SetStatus(KirbyStatus::Idle);
		}
	}

	if (kirby->GetKirbyStatus() == KirbyStatus::Attacked)
	{
		Attacked();
		Vec3 v = Vec3{ velocity.x, gravity * mass + velocity.y, velocity.z };
		kirby->SetPositionWithPlus(v * Time->GetDeltaTime());
		return;
	}

	if (KirbyStatusCheck({ KirbyStatus::ElectricShock, KirbyStatus::SuckedElectricShock }))
	{
		AttackedElecricOrFire();
		Vec3 v = Vec3{ velocity.x, gravity * mass + velocity.y, velocity.z };
		kirby->SetPositionWithPlus(v * Time->GetDeltaTime());
		return;
	}

	if (KirbyStatusCheck({ KirbyStatus::ShockedLand }))
	{
		ShockedLand();
		Vec3 v = Vec3{ velocity.x, gravity * mass + velocity.y, velocity.z };
		kirby->SetPositionWithPlus(v * Time->GetDeltaTime());
		return;
	}

	if (kirby->GetKirbyStatus() == KirbyStatus::SpitOut)
	{
		if (maintainSpitStatus < 0.1f)
		{
			maintainSpitStatus += Time->GetDeltaTime();
			Vec3 v = Vec3{ velocity.x, gravity * mass + velocity.y, velocity.z };
			kirby->SetPositionWithPlus(v * Time->GetDeltaTime());
			return;
		}
		else
		{
			maintainSpitStatus = 0.f;
			SetStatus(KirbyStatus::Falling);
		}
	}

	if (kirby->GetKirbyStatus() == KirbyStatus::Attacking)
	{
		if (!grounded)
		{
			velocity.y = -2.f;
		}
		Attack();
	}

	if (grounded && KirbyStatusCheck({ KirbyStatus::Falling, KirbyStatus::SuckedFalling }))
	{
		if (Input->GetButton(KEY_TYPE::LEFT) || Input->GetButton(KEY_TYPE::RIGHT))
		{
			if (beforeFallStatus == KirbyStatus::Dash)
			{
				SetStatus(KirbyStatus::Dash);
			}
			else if (beforeFallStatus == KirbyStatus::BendOver)
			{
				if (beforeJumpStatus == KirbyStatus::Dash)
				{
					SetStatus(KirbyStatus::Dash);
				}
				else
				{
					SetStatus(KirbyStatus::Walking);
				}
			}
			else
			{
				SetStatus(KirbyStatus::Walking);
			}
		}
		else
		{
			SetIdle();
		}
		wasBendOver = false;
	}

	if (kirby->GetCollideWithLeftRampGrass() || kirby->GetCollideWithRightRampGrass())
	{
		SetStatus(kirby->GetKirbyStatus());
	}
}

void Ability::Update(Kirby* kirby)
{
	CheckStatusWhenUpdate();

	PushLeftArrow(kirby);
	RightArrowKey(kirby);
	PushSpace();
	PushDownArrow(kirby);
	PushUpArrow(kirby);
	PushS(kirby);
	PushQ(kirby);

	if (KirbyStatusCheck({ KirbyStatus::UpLadder, KirbyStatus::DownLadder }))
	{
		Vec3 v = Vec3{ velocity.x, velocity.y, velocity.z };
		kirby->SetPositionWithPlus(v * Time->GetDeltaTime());
	}
	else
	{
		Vec3 v = Vec3{ velocity.x, gravity * mass + velocity.y, velocity.z };
		kirby->SetPositionWithPlus(v * Time->GetDeltaTime());
	}
}

bool Ability::KirbyStatusCheck(const vector<KirbyStatus>& statuses)
{
	KirbyStatus presentStatus = kirby->GetKirbyStatus();

	for (const KirbyStatus status : statuses)
	{
		if (presentStatus == status) return true;
	}
	return false;
}

void Ability::Die()
{
	dieTimeCalc += Time->GetDeltaTime();

	if (1.f <= dieTimeCalc && dieTimeCalc < 1.2f)
	{
		velocity.y = 6.5f;
	}
	else if (1.2f <= dieTimeCalc)
	{
		velocity.y = -8.5f;
	}
}

void Ability::ChangeVelocityOnTheRampGround(Direction goingDirection)
{
	if (kirby->GetCollideWithLeftRampGrass() || kirby->GetCollideWithRightRampGrass())
	{
		Vec3 directionVec;
		if (goingDirection == Direction::LEFT)
		{
			directionVec = kirby->GetTiltLeftDirection();
		}
		else
		{
			directionVec = kirby->GetTiltRightDirection();
		}

		float a = velocity.x / directionVec.x;
		velocity.y = directionVec.y * a;
	}
	else
	{
		if (grounded)
		{
			velocity.y = 0.f;
		}
	}
}

void Ability::test(Direction direction)
{
	if (KirbyStatusCheck({ KirbyStatus::FloatingUp, KirbyStatus::Falling, KirbyStatus::SuckedFalling}))
	{
		velocity.x = direction == Direction::LEFT ? -3.f : 3.f;
	}
	else if(KirbyStatusCheck({ KirbyStatus::Jumping, KirbyStatus::SuckedJumping }))
	{
		velocity.x = direction == Direction::LEFT ? -5.f : 5.f;
	}
	else if (KirbyStatusCheck({ KirbyStatus::FloatingDown }))
	{
		if (grounded)
		{
			velocity.x = direction == Direction::LEFT ? -maxWalkVelocity : maxWalkVelocity;
		}
		else
		{
			velocity.x = direction == Direction::LEFT ? -3.f : 3.f;
		}
	}
	else if (KirbyStatusCheck({ KirbyStatus::Walking, KirbyStatus::SuckedWalking }))
	{
		velocity.x = direction == Direction::LEFT ? -maxWalkVelocity : maxWalkVelocity;
	}
	else if (KirbyStatusCheck({ KirbyStatus::Dash, KirbyStatus::SuckedDash }))
	{
		velocity.x = direction == Direction::LEFT ? -maxDashVelocity : maxDashVelocity;
	}

	if (KirbyStatusCheck({ KirbyStatus::Jumping, KirbyStatus::SuckedJumping})) return;		// 점프할때도 경사로 조정을 하니 점프가 안돼서 return

	ChangeVelocityOnTheRampGround(direction);
}

void Ability::PushLeftArrow(Kirby* kirby)
{
	if (blockLeftSide) return;
	if (KirbyStatusCheck({ KirbyStatus::Attacking, KirbyStatus::Attacked, KirbyStatus::Sucking, KirbyStatus::Foldding })) return;

	if (Input->GetButton(KEY_TYPE::LEFT))
	{
		if (blockRightSide)
			blockRightSide = false;

		test(Direction::LEFT);
	}
	else
	{
		potentialOfDashLeft -= Time->GetDeltaTime();
		if (potentialOfDashLeft <= 0.f) leftArrow = 0;
	}

	if (Input->GetButtonDown(KEY_TYPE::LEFT))
	{
		kirby->SetDirection(Direction::LEFT);

		if (!KirbyStatusCheck({ KirbyStatus::FloatingUp, KirbyStatus::FloatingDown, KirbyStatus::Jumping, KirbyStatus::SuckedJumping, KirbyStatus::Falling, KirbyStatus::SuckedFalling }))
		{
			if (KirbyStatusCheck({ KirbyStatus::UpLadder, KirbyStatus::DownLadder }))
			{
				SetStatus(KirbyStatus::Falling);
				kirby->GetAnimator()->ActivateAnimator();
			}
			else
			{
				potentialOfDashLeft = 0.4f;
				if (leftArrow == 0) leftArrow++;
				else if (leftArrow == 1 && potentialOfDashLeft > 0.f) leftArrow++;

				if (leftArrow == 1)
				{
					SetStatus(KirbyStatus::Walking);
				}
				else
				{
					SetStatus(KirbyStatus::Dash);
				}
			}
		}
	}

	if (Input->GetButtonUp(KEY_TYPE::LEFT))
	{
		velocity.x = 0.f;
		if (!Input->GetButton(KEY_TYPE::UP))
		{
			velocity.y = 0.f;
		}
		if (KirbyStatusCheck({ KirbyStatus::Walking, KirbyStatus::SuckedWalking, KirbyStatus::Dash, KirbyStatus::SuckedDash }))
		{
			SetIdle(); // 공격중이나 맞고있을때 못바꾸도록 해야될듯
		}
	}
}

void Ability::RightArrowKey(Kirby* kirby)
{
	if (blockRightSide) 
		return;

	if (KirbyStatusCheck({ KirbyStatus::Attacking, KirbyStatus::Attacked, KirbyStatus::Sucking, KirbyStatus::Foldding }))	return;

	if (Input->GetButton(KEY_TYPE::RIGHT))
	{
		if (blockLeftSide)
			blockLeftSide = false;

		test(Direction::RIGHT);
	}
	else
	{
		potentialOfDashRight -= Time->GetDeltaTime();
		if (potentialOfDashRight <= 0.f) rightArrow = 0;
	}

	if (Input->GetButtonDown(KEY_TYPE::RIGHT))
	{
		kirby->SetDirection(Direction::RIGHT);

		if (!KirbyStatusCheck({ KirbyStatus::FloatingUp, KirbyStatus::FloatingDown, KirbyStatus::Jumping, KirbyStatus::SuckedJumping, KirbyStatus::Falling, KirbyStatus::SuckedFalling }))
		{
			if (KirbyStatusCheck({ KirbyStatus::UpLadder, KirbyStatus::DownLadder }))
			{
				SetStatus(KirbyStatus::Falling);
				kirby->GetAnimator()->ActivateAnimator();
			}
			else
			{
				potentialOfDashRight = 0.4f;
				if (rightArrow == 0)
					rightArrow++;
				else if (rightArrow == 1 && potentialOfDashRight > 0.f)
					rightArrow++;

				if (rightArrow == 1)
				{
					SetStatus(KirbyStatus::Walking);
				}
				else
				{
					SetStatus(KirbyStatus::Dash);
				}
			}
		}
	}

	if (Input->GetButtonUp(KEY_TYPE::RIGHT))
	{
		velocity.x = 0.f;
		if (!Input->GetButton(KEY_TYPE::UP))
		{
			velocity.y = 0.f;
		}

		if (KirbyStatusCheck({ KirbyStatus::Walking, KirbyStatus::SuckedWalking, KirbyStatus::Dash, KirbyStatus::SuckedDash }))
		{
			SetIdle(); // 공격중이나  맞고있을때 못바꾸도록 해야될듯
		}
	}
}

void Ability::PushDownArrow(Kirby* kirby)
{
	if (Input->GetButtonDown(KEY_TYPE::DOWN))
	{
		if (KirbyStatusCheck({ KirbyStatus::FloatingUp, KirbyStatus::FloatingDown })) return;

		if (grounded)
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

void Ability::PushSpace()
{
	if (KirbyStatusCheck({ KirbyStatus::UpLadder, KirbyStatus::DownLadder })) return;

	if (KirbyStatusCheck({ KirbyStatus::Jumping, KirbyStatus::SuckedJumping }))
	{
		if (Input->GetButton(KEY_TYPE::SPACE))
		{
			if (maintainSpace >= 0.6f)
			{
 				//SetStatus(KirbyStatus::BendOver);
				maintainSpace = 0.f;
				return;
			}
			else
			{
				maintainSpace += Time->GetDeltaTime();
				jumpPower = jumpPower < maxJumpVelocity ? jumpPower + 2.f : maxJumpVelocity;
			}
		}
	}

	if (Input->GetButtonDown(KEY_TYPE::SPACE) && grounded)
	{
 		SetStatus(KirbyStatus::Jumping);
	}

	if (Input->GetButtonUp(KEY_TYPE::SPACE) && KirbyStatusCheck({ KirbyStatus::Jumping, KirbyStatus::SuckedJumping }))
	{
		SetStatus(KirbyStatus::BendOver);
		maintainSpace = 0.f;
	}
}

void Ability::PushUpArrow(Kirby* kirby) 
{
	if (Input->GetButtonDown(KEY_TYPE::UP))
	{
		if (!kirby->GetSucked())
		{
			if (KirbyStatusCheck({ KirbyStatus::Attacking, KirbyStatus::Attacked })) return;

			if (kirby->GetFrontLadder() && !KirbyStatusCheck({ KirbyStatus::FloatingUp, KirbyStatus::FloatingDown }))
			{
				SetStatus(KirbyStatus::UpLadder);
				velocity.y = 4.5f;
				kirby->GetAnimator()->ActivateAnimator();
			}
			else if (kirby->GetFrontDoor())
			{
				SetStatus(KirbyStatus::EnterDoor);
			}
			else
			{
				SetStatus(KirbyStatus::FloatingUp);
				velocity.y = 5.5f;
				mass = 1.f;
			}
		}
	}

	if (Input->GetButton(KEY_TYPE::UP))
	{
		if (KirbyStatusCheck({ KirbyStatus::UpLadder }) && kirby->GetOnTheLadderTop())
		{
			SetStatus(KirbyStatus::Jumping);
			jumpPower = 8.5f;
			jumpTime = 0.f;
			gravity = -1.f;
			kirby->GetAnimator()->ActivateAnimator();
		}
	}

	if (Input->GetButtonUp(KEY_TYPE::UP))
	{
		if (KirbyStatusCheck({ KirbyStatus::FloatingUp }))
		{
			SetStatus(KirbyStatus::FloatingDown);
			velocity.y = 0.f;
		}
		else if (KirbyStatusCheck({ KirbyStatus::UpLadder}))
		{
			velocity.y = 0.f;
			kirby->GetAnimator()->InactivateAnimator();
		}
	}
}

void Ability::PushS(Kirby* kirby)
{
	if (KirbyStatusCheck({ KirbyStatus::UpLadder, KirbyStatus::DownLadder })) return;

	if (Input->GetButtonDown(KEY_TYPE::S))
	{
		pushS = true;

		if (KirbyStatusCheck({ KirbyStatus::FloatingDown , KirbyStatus::FloatingUp }))
		{
			BreathOut();
		}
		else
		{
			if (!KirbyStatusCheck({ KirbyStatus::Attacking }))
			{
				kirby->AddGameObjectToScene(weapon);
				Vec3 kirbyPosition = kirby->GetTransform()->GetPosition();
				queue<KEY_TYPE> keyQueue;
				keyQueue.push(KEY_TYPE::S);

				weapon->Trigger(keyQueue, kirbyPosition, kirby->GetDirecton(), kirby);

				SetStatus(KirbyStatus::Attacking);
			}
		}
	}

	if (Input->GetButtonUp(KEY_TYPE::S))
	{
		pushS = false;
	}
}

void Ability::PushQ(Kirby* kirby)
{
	if (Input->GetButtonDown(KEY_TYPE::Q))
	{
		Direction starDirection = kirby->GetDirecton() == Direction::LEFT ? Direction::RIGHT : Direction::LEFT;
		GAME->GetSceneManager()->GetActiveScene()->AddGameObject(new AbilityStar(this, starDirection));
		GAME->GetSceneManager()->GetActiveScene()->AddGameObject(Pool->GetEffectObject(L"kirbyBackToNormal", kirby->GetMidstPosition()));

		Ability* normalAbility = Pool->GetAbilityObject(L"Normal");
		kirby->SetAbility(normalAbility);
		if (weapon != nullptr)
		{
			kirby->DeleteChild(weapon);
		}
		normalAbility->SetKirby(kirby);

		ChangeKirbyAnimationGather(normalAbility->GetAnimationGatherName());
		kirby->GetAnimator()->SetPresentAnimation(50);
		uint8 animNum = static_cast<uint8>(kirby->GetKirbyStatus());

		if (kirby->GetDirecton() == Direction::RIGHT)
		{
			animNum++;
		}
		kirby->GetAnimator()->SetPresentAnimation(animNum);
		ChangeKirbyMesh(normalAbility->GetMeshName());
	}
}

void Ability::SetGrounded(bool _grounded) 
{ 
	if (grounded != _grounded)
	{
		grounded = _grounded;
		if (grounded)
		{
			velocity.y = 0.f;
			gravity = 0.f;
			jumpTime = 0.f;
			jumpPower = 0.f;
		}
		else
		{
			gravity = -1.f;
			
			if (KirbyStatusCheck({ KirbyStatus::Walking, KirbyStatus::Dash, KirbyStatus::Idle }))
			{
				SetStatus(KirbyStatus::Falling);
				//mass = 3.f;
				//velocity.y = -3.f;
			}
			else if (KirbyStatusCheck({ KirbyStatus::SuckedWalking, KirbyStatus::SuckedDash, KirbyStatus::SuckedIdle }))
			{
				SetStatus(KirbyStatus::SuckedFalling );
				mass = 1.f;
			}
		}
	}
}

void Ability::BlockTop()
{
	if (KirbyStatusCheck({ KirbyStatus::Jumping, KirbyStatus::SuckedJumping }))
	{
		SetStatus(KirbyStatus::Falling);
	}
	else if (KirbyStatusCheck({ KirbyStatus::FloatingUp }))
	{
		velocity.y = 0.f;
	}
}

void Ability::BlockLeft()
{
	velocity.x = 0.f;
	blockLeftSide = true;
	if (grounded && kirby->GetDirecton() == Direction::LEFT)
	{
		if (KirbyStatusCheck({ KirbyStatus::Walking, KirbyStatus::Dash, KirbyStatus::SuckedWalking, KirbyStatus::SuckedDash }))
		{
			SetStatus(KirbyStatus::Idle);
		}
	}
}

void Ability::BlockRight()
{
	blockRightSide = true;
	velocity.x = 0.f;
	if (grounded && kirby->GetDirecton() == Direction::RIGHT)
	{
		if (KirbyStatusCheck({ KirbyStatus::Walking, KirbyStatus::Dash, KirbyStatus::SuckedWalking, KirbyStatus::SuckedDash }))
		{
			SetStatus(KirbyStatus::Idle);
		}
	}
}

void Ability::NotCollideAtAllWithGroundBottom()
{
	SetGrounded(false);
}

void Ability::NotCollideAtAllWithGroundSide()
{
	blockLeftSide = false;
	blockRightSide = false;
}

void Ability::CollideWithMonster(Direction direction, float damage, AttackType attackType)
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

		Direction starDirection = kirby->GetDirecton() == Direction::LEFT ? Direction::RIGHT : Direction::LEFT;
		GAME->GetSceneManager()->GetActiveScene()->AddGameObject(new AbilityStar(this, starDirection));
	}
	Ability* normalAbility = Pool->GetAbilityObject(L"Normal");
	kirby->SetAbility(normalAbility);
	normalAbility->SetKirby(kirby);

	ChangeKirbyAnimationGather(normalAbility->GetAnimationGatherName());
	kirby->GetAnimator()->SetPresentAnimation(50);
	uint8 animNum = static_cast<uint8>(kirby->GetKirbyStatus());

	if (kirby->GetDirecton() == Direction::RIGHT)
	{
		animNum++;
	}
	kirby->GetAnimator()->SetPresentAnimation(animNum);
	ChangeKirbyMesh(normalAbility->GetMeshName());
}

void Ability::CollideWithSuckedAbilityMonster(Ability* ability)
{
	if (grounded)
	{
		kirby->SetKirbyStatus(KirbyStatus::SuckedIdle);
	}
	else
	{
		kirby->SetKirbyStatus(KirbyStatus::SuckedFalling);
	}

	tempAbility = ability;
}

void Ability::BreathOut()
{
	kirby->SetKirbyStatus(KirbyStatus::SpitOut);

	//숨공격 만들기
	BreathWeapon* breathWeapon = new BreathWeapon();
	kirby->AddGameObjectToScene(breathWeapon);
	Vec3 kirbyPosition = kirby->GetTransform()->GetPosition();

	queue<KEY_TYPE> keyQueue;
	keyQueue.push(KEY_TYPE::S);
	breathWeapon->Trigger(keyQueue, kirbyPosition, kirby->GetDirecton(), kirby);
}

void Ability::Attacked()
{
	if (maintainAttackedStatus >= 1.04f)
	{
		if (grounded)
		{
			/*velocity.x = 0.f;*/
			SetIdle();
		}
		else
		{
			SetStatus(KirbyStatus::Falling);
		}
		maintainAttackedStatus = 0.f;
	}
	else
	{
		maintainAttackedStatus += Time->GetDeltaTime();

		if (blockLeftSide || blockRightSide) return;

		if (maintainAttackedStatus <= 0.f && maintainAttackedStatus <= 0.8f)
		{
			velocity.x = kirby->GetDirecton() == Direction::LEFT ? 1.8f : -1.8f;
		}
		else
		{
			velocity.x = kirby->GetDirecton() == Direction::LEFT ? 0.5f : -0.5f;
		}
	}
}

void Ability::AttackedElecricOrFire()
{
	maintainAttackedStatus += Time->GetDeltaTime();

	if (maintainAttackedStatus >= 0.f && maintainAttackedStatus < 0.4f)
	{
		velocity.x = kirby->GetDirecton() == Direction::LEFT ? 1.5f : -1.5f;
		velocity.y = 1.5f;
	}
	else if (maintainAttackedStatus >= 0.2f && maintainAttackedStatus < 0.8f)
	{
		velocity.x = 0.f;
		velocity.y = -1.5f;
	}


	if (maintainAttackedStatus > 0.4f && grounded)
	{
		maintainAttackedStatus = 0.f;
		SetStatus(KirbyStatus::ShockedLand);
	}

	if (blockLeftSide || blockRightSide)
	{
		velocity.x = 0.f;
	}
}

void Ability::ShockedLand()
{
	landTimeCalc += Time->GetDeltaTime();

	if (0.f <= landTimeCalc && landTimeCalc < 0.2f)
	{
		velocity.y = 1.8f;
	}
	else if (0.2f <= landTimeCalc && landTimeCalc < 0.5f)
	{
		velocity.y = -1.5f;
	}
	else if (0.6f <= landTimeCalc)
	{
		velocity.y = 1.5f;
		landTimeCalc = 0.f;
		SetStatus(KirbyStatus::BendOver);
	}
}

void Ability::SetIdle()
{
	velocity.x = 0.f;
	velocity.y = 0.f;

	SetStatus(KirbyStatus::Idle);
}

void Ability::SetStatus(KirbyStatus status)
{
	bool isSucked = kirby->GetSucked();
	bool isOnLeftTilt = kirby->GetCollideWithLeftRampGrass();
	bool isOnRightTilt = kirby->GetCollideWithRightRampGrass();
	KirbyStatus newStatus;

	switch (status)
	{
	case KirbyStatus::Idle:
		if (isSucked)
		{
			if (isOnLeftTilt)
			{
				newStatus = KirbyStatus::SuckedIdleLeftTilt;
			}
			else if (isOnRightTilt)
			{
				newStatus = KirbyStatus::SuckedIdleRightTilt;
			}
			else
			{
				newStatus = KirbyStatus::SuckedIdle;
			}
		}
		else if (isOnLeftTilt)
		{
			newStatus = KirbyStatus::IdleLeftTilt;
		}
		else if (isOnRightTilt)
		{
			newStatus = KirbyStatus::IdleRightTilt;
		}
		else
		{
			newStatus = KirbyStatus::Idle;
		}
		break;
	case KirbyStatus::Walking:
		newStatus = isSucked ? KirbyStatus::SuckedWalking : KirbyStatus::Walking;
		break;
	case KirbyStatus::Dash:
		newStatus = isSucked ? KirbyStatus::SuckedDash : KirbyStatus::Dash;
		{
			wstring effect = kirby->GetDirecton() == Direction::LEFT ? L"kirbyDashWindLeft" : L"kirbyDashWindRight";
			GAME->GetSceneManager()->GetActiveScene()->AddGameObject(Pool->GetEffectObject(effect, kirby->GetMidstPosition()));
		}
		break;
	case KirbyStatus::Falling:
		mass = isSucked ? 1.f : 3.f;
		velocity.y = -3.f;
		beforeFallStatus = kirby->GetKirbyStatus();
		newStatus = isSucked ? KirbyStatus::SuckedFalling : KirbyStatus::Falling;
		break;
	case KirbyStatus::Jumping:
		beforeJumpStatus = kirby->GetKirbyStatus();
		newStatus = isSucked ? KirbyStatus::SuckedJumping : KirbyStatus::Jumping;
		break;
	case KirbyStatus::Attacked:
		newStatus = isSucked ? KirbyStatus::SuckedAttacked : KirbyStatus::Attacked;
		break;
	case KirbyStatus::ElectricShock:
		newStatus = isSucked ? KirbyStatus::SuckedElectricShock : KirbyStatus::ElectricShock;
		break;
	case KirbyStatus::Die:
		newStatus = status;
		velocity.x = 0.f;
		kirby->GetCollider()->Inactivate();
		break;
	default:
		newStatus = status;
		break;
	}

	if (KirbyStatusCheck({ KirbyStatus::Falling, KirbyStatus::SuckedFalling }))
	{
		Vec3 effectPosition = kirby->GetMidstPosition() + Vec3{ 0.5f, -0.7f, 0.f };
		GAME->GetSceneManager()->GetActiveScene()->AddGameObject(Pool->GetEffectObject(L"landingStar", effectPosition));
	}
	kirby->SetKirbyStatus(newStatus);
}

void Ability::ChangeKirbyAnimationGather(string gatherName)
{
	kirby->GetAnimator()->SetAnimationGater(gatherName);
}

void Ability::ChangeKirbyMesh(wstring meshName)
{
	Mesh* mesh = GAME->GetResourceManager()->Get<Mesh>(meshName);
	kirby->GetMeshRenderer()->SetMesh(mesh);
}