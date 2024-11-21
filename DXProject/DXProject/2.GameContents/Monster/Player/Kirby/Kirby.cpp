#include "pch.h"
#include "Ability.h"
#include "Animator.h"
#include "Collider.h"
#include "DotCollider.h"
#include "EmptyObject.h"
#include "KirbyCollisionBehavior.h"
#include "Kirby.h"
#include "MeshRenderer.h"
#include "RectCollider.h"
#include "SceneManager.h"
#include "Scene.h"

Kirby::Kirby(Ability* _ability) : ability(_ability)
{
	{
		Collider* standardCollider = new RectCollider("KirbyBasic");
		standardCollider->AddCheckingTag("RectGrassGround");
		standardCollider->AddCheckingTag("Door");
		standardCollider->AddCheckingTag("Button");
		standardCollider->AddCheckingTag("ThinFloor");
		standardCollider->AddCheckingTag("Ladder");
		standardCollider->AddCheckingTag("LadderTop");

		standardCollider->SetCollisionBehavior(new KirbyCollisionBehavior());
		vector<Vec3> vertices(5);

		vertices[0] = Vec3(-0.3f, 0.6f, 0.f);
		vertices[1] = Vec3(0.3f, 0.6f, 0.f);
		vertices[2] = Vec3(0.3f, 0.f, 0.f);
		vertices[3] = Vec3(-0.3f, 0.f, 0.f);
		vertices[4] = Vec3(-0.3f, 0.6f, 0.f);
		standardCollider->SetVertices(vertices);
		this->AddComponent(standardCollider);
	}

	ability->SetKirby(this);
}

Kirby::~Kirby()
{

}

void Kirby::UpdateObject()
{
	if (!collideWithBottomGrass && !collideWithLeftRampGrass && !collideWithRightRampGrass)
	{
		ability->SetGrounded(false);
	}

	if (invincible)
	{
		if (invincibleTimeCalc < invincibleTime)
		{
			invincibleTimeCalc += Time->GetDeltaTime();
		}
		else
		{
			invincibleTimeCalc = 0.f;
			invincible = false;
		}
	}

	if (insideMonsterMouth)
	{
		if (insideMonsterMouthTimeCalc < insideMonsterMouthTime)
		{
			insideMonsterMouthTimeCalc += Time->GetDeltaTime();
		}
		else
		{
			insideMonsterMouth = false;
			insideMonsterMouthTimeCalc = 0.f;
			OutsideMonsterMouth();
		}
	}
	else
	{
		ability->Update(this);
	}
}


void Kirby::SetKirbyStatus(KirbyStatus _status)
{
	status = _status;

	uint8 animNum = static_cast<uint8>(status);

	if (direction == Direction::RIGHT)
	{
		animNum++;
	}

	GetAnimator()->SetPresentAnimation(animNum);
}

void Kirby::SetPositionWithPlus(Vec3 amount)
{
	Vec3 presentPosition = GetTransform()->GetPosition();
	GetTransform()->SetPosition(presentPosition + amount);
}

void Kirby::SetDirection(Direction _direction) 
{ 
	if (direction != _direction)
	{
		direction = _direction;

		SetKirbyStatus(status);
	}
}

void  Kirby::SetGrounded(bool _gronded)
{
	ability->SetGrounded(_gronded);
}

void Kirby::AlertBlockSide(CollisionDirection direction)
{
	if (direction == CollisionDirection::Left)
	{
		ability->BlockLeft();
	}
	else
	{
		ability->BlockRight();
	}
}

void Kirby::AlertBlockTop()
{
	ability->BlockTop();
}

void Kirby::NotCollideAtAllWithGroundBottom()
{
	ability->NotCollideAtAllWithGroundBottom();
}

void Kirby::NotCollideAtAllWithGroundSide()
{
	ability->NotCollideAtAllWithGroundSide();
}

void Kirby::AlertCollideWithMonster(Direction direction, float damage, AttackType attackType)
{
	ability->CollideWithMonster(direction, damage, attackType);
	invincible = true;
}

void Kirby::AlertCollideWithSuckedAbilityMonster(Ability* _ability)
{
	SetSucked(true);
	SetCollideWithAbilityMonster(true);
	ability->CollideWithSuckedAbilityMonster(_ability);
}

void Kirby::AlertCollideWithSuckedStarMonster()
{
	SetSucked(true);
	SetCollideWithAbilityMonster(false);
	if (ability->GetGrounded())
	{
		SetKirbyStatus(KirbyStatus::SuckedIdle);
	}
	else
	{
		SetKirbyStatus(KirbyStatus::SuckedFalling);
	}
}

float Kirby::DecreaseBlood(float amount) 
{
	blood -= amount;
	return blood; 
}

float Kirby::IncreaseBlood(float amount)
{
	blood += amount;
	if (blood > 100.f)
	{
		blood = 100.f;
	}

	return blood;
}

Vec3 Kirby::GetVelocity() const
{ 
	return ability->GetVelocity(); 
}
void Kirby::InsideMonsterMouth(float attackTime)
{
	insideMonsterMouth = true;
	GetMeshRenderer()->InactiveMeshRenderer();
	GetCollider()->Inactivate();
	insideMonsterMouthTime = attackTime;
}

void Kirby::OutsideMonsterMouth()
{
	GetMeshRenderer()->ActiveMeshRenderer();
	GetCollider()->Activate();
	ability->CollideWithMonster(direction, 20.f, AttackType::Normal);
	invincible = true;
}

wstring  Kirby::GetAbilityInfoTexture() const
{ 
	return ability->GetAbilityIconTextureName(); 
}

void Kirby::AlertChangeScene(bool revive)
{
	if (revive)
	{
		GetCollider()->Activate();
		blood = 100.f;
		life--;
	}

	if (ability->GetGrounded())
	{
		SetKirbyStatus(KirbyStatus::Idle);
	}
	else
	{
		SetKirbyStatus(KirbyStatus::Falling);
	}
}