#include "pch.h"
#include "00. Engine/Component/Animator/Animator.h"
#include "00. Engine/Component/Collider/Collider.h"
#include "02. GameContents/Monster/Variety/Monster.h"
#include "00. Engine/Manager/ResourceManager/Resource/Material.h"
#include "00. Engine/Manager/ResourceManager/Resource/Mesh.h"
#include "00. Engine/Component/MeshRenderer/MeshRenderer.h"
#include "00. Engine/Manager/SceneManager/SceneManager.h"
#include "02. GameContents/Item/Umbrella.h"
#include "02. GameContents/Weapon/Weapon.h"
#include  <string>

Monster::Monster(wstring _mesh, wstring _material)
{
	// add MeshRenderer
	MeshRenderer* meshRenderer = new MeshRenderer();
	AddComponent(meshRenderer);
	auto mesh = GGameManager->GetResourceManager()->Get<Mesh>(_mesh);
	meshRenderer->SetMesh(mesh);
	auto material = GGameManager->GetResourceManager()->Get<Material>(_material);
	meshRenderer->SetMaterial(material);
	meshRenderer->SetRenderingOrder(3);

	// add Animator
	Animator* animator = new Animator();
	AddComponent(animator);
}

void Monster::InitMonster(Vec3 _initPosition, MonsterAI* _monsterAI)
{
	initPosition = _initPosition;
	blood = initBlood;
	GetTransform()->SetPosition(initPosition);
	monsterAI = _monsterAI;

	ChangeBehavior(monsterAI->GetPresentBehavior());
	initStatus = status;
	if (haveUmbrella)
	{
		SetStatus(MonsterStatus::UmbrellaDown);
		umbrella = new Umbrella();
		AddChild(umbrella);
		umbrella->SetParent(this);
		umbrella->GetTransform()->SetPosition(initPosition + Vec3{ 0.f, 0.8f, 0.f });
		AddGameObjectToScene(umbrella);
	}

	uint8 animNum = static_cast<uint8>(status);

	if (direction == Direction::RIGHT)
	{
		animNum++;
	}
	GetAnimator()->SetPresentAnimation(animNum);
}

Monster::~Monster()
{

}

void Monster::UpdateObject()
{
	if (CheckNeedRespone()) return;

	monsterAI->UpdateBehavior(this, attacked, sucked, needStartAttack);		// 몬스터 행동순서를 전략에 위임
	attacked = false;
	sucked = false;

	Vec3 v = Vec3{ velocity.x, gravity + velocity.y, velocity.z };
	Vec3 presentPosition = GetTransform()->GetPosition();
	GetTransform()->SetPosition(presentPosition + v * Time->GetDeltaTime());
}

bool Monster::CheckNeedRespone()
{
	Vec3 kirbyPosition = GAME->GetSceneManager()->GetKirbyPosition();
	float distanceWithKirby = abs(kirbyPosition.x - GetTransform()->GetPosition().x);

	if (distanceWithKirby >= 14.f)
	{
		if (die)
		{
			GetMeshRenderer()->ActiveMeshRenderer();
		}
		GetTransform()->SetPosition(initPosition);
		blood = initBlood;
		die = false;
		velocity = Vec3{ 0.f, 0.f, 0.f };
		SetStatus(initStatus);

		Collider* collider = GetCollider();
		if (collider != nullptr)
		{
			collider->Activate();
		}

		return true;
	}

	if (die) return true;
	return false;
}

void Monster::SetDirection(Direction _direction, bool maintainAnimStatus)
{
	if (direction != _direction)
	{
		direction = _direction;

		uint8 animNum = static_cast<uint8>(status);

		if (direction == Direction::RIGHT)
		{
			animNum++;
		}

		GetAnimator()->SetPresentAnimation(animNum, maintainAnimStatus);
	}
}

void Monster::SetStatus(MonsterStatus _status, bool maintainAnimStatus)
{
	if (status == _status) return;
	if (_status == MonsterStatus::Fall)
	{
		beforeFallingStatus = status;
	}
	else if (_status == MonsterStatus::Jump)
	{
		jumpTimeCalc = 0.f;
	}

	if (status == MonsterStatus::Attack)
	{
		if (weapon != nullptr)
		{
			weapon->StopAttack();
		}
	}

	status = _status;

	uint8 animNum = static_cast<uint8>(status);

	if (direction == Direction::RIGHT)
	{
		animNum++;
	}

	GetAnimator()->SetPresentAnimation(animNum);
}

void Monster::AlertAttacked(Direction _attackedDirection, float damage)
{
	if (attackedTimeCalc == 0.0f)
	{
		attackedDirection = _attackedDirection;
		attacked = true;
		blood -= damage;
		GAME->GetSceneManager()->SetMonsterRemainBloodRatio(blood / initBlood);
	}
}

void Monster::AlertSucked(Direction _suckedDirection, Vec3 kirbyPosition)
{
	if (!die)
	{
		suckedPosition = kirbyPosition;
		suckedDirection = _suckedDirection;
		sucked = true;
	}
}

void Monster::AlertStartAttack()
{
	needStartAttack = true;
}

void Monster::Idle()
{
	if (idleTimeCalc >= idleTime)
	{
		endOfBehaivor = true;
		idleTimeCalc = 0.f;
	}
	else
	{
		idleTimeCalc += Time->GetDeltaTime();
	}
}

void Monster::Jump()
{
	if (jumpTimeCalc == 0.f)
	{
		SetStatus(MonsterStatus::Jump);
	}

	if(status == MonsterStatus::Jump || (beforeFallingStatus == MonsterStatus::Jump && status == MonsterStatus::Fall))
	{
		jumpTimeCalc += Time->GetDeltaTime() * 15;
		velocity.y = jumpTimeCalc * (gravity / 2) + jumpPower;

		if (velocity.y < 0)
		{
			SetStatus(MonsterStatus::Fall);
		}
	}
}

void Monster::Attacked()
{
	if (attackedTimeCalc > attackedMaintainTime)
	{
		attackedTimeCalc = 0.f;

		if (haveUmbrella)  // 우산을 들고있는 경우(우산 놓아줌)
		{
			haveUmbrella = false;

			if (umbrella != nullptr)
			{
				umbrella->ReleaseFromMonster();
				umbrella = nullptr;
			}
		}

		// 피 양 계산해서 분기 만들기
		if (blood <= 0.f)
		{
			Die();
			GAME->GetSceneManager()->GetActiveScene()->AddGameObject(Pool->GetEffectObject(L"monsterExplosionBasic", GetMidstPosition()));
		}
		else
		{
			endOfBehaivor = true;
			if (grounded)
			{
				velocity.x = 0.f;
				velocity.y = 0.f;
				SetStatus(initStatus);
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
		attackedTimeCalc += Time->GetDeltaTime();

		if (attackedDirection == Direction::LEFT)
		{
			velocity = Vec3{ 1.2f, 0.2f, 0.f };
		}
		else
		{
			velocity = Vec3{ -1.2f, 0.2f, 0.f };
		}
	}
}

void Monster::Die()
{
	GetMeshRenderer()->InactiveMeshRenderer();
	Collider* collider = GetCollider();
	if (collider != nullptr)
	{
		collider->Inactivate();
	}
	if (weapon != nullptr)
	{
		weapon->StopAttack();
	}

	die = true;
	endOfBehaivor = true;
	attacked = false;
	sucked = false;
}

void Monster::Sucked()
{
	Vec3 monsterPosition = GetTransform()->GetPosition();
	velocity.x = (suckedPosition.x - monsterPosition.x) * 4.5f;
	velocity.y = (suckedPosition.y - monsterPosition.y) * 4.5f;
}

void Monster::NotCollideAtAllWithGroundBottom()
{
	SetGrounded(false);
}

void Monster::SetGrounded(bool _grounded)
{
	if (grounded != _grounded)
	{
		grounded = _grounded;
		if (grounded)
		{
			velocity.y = 0.f;
			gravity = 0.f;

			if (haveUmbrella)
			{
				if (umbrella != nullptr)
				{
					umbrella->ReleaseFromMonster();
					umbrella = nullptr;
				}
			}
		}
		else
		{
			gravity = -1.f;
		}
	}
}

void Monster::UmbrellaDown()
{
	if (umbrellaTimeCalc < umbrellaTime)
	{
		umbrellaTimeCalc += Time->GetDeltaTime();
		velocity.x = umbrellaFront ? 1.f : -1.f;
	}
	else
	{
		umbrellaTimeCalc = 0.f;
		umbrellaFront = umbrellaFront ? false : true;
	}

	if (grounded)
	{
		velocity.y = 0.f;
		velocity.x = 0.f;
		haveUmbrella = false;
		endOfBehaivor = true;
	}
}

void Monster::AlertBlockSide(Direction _blockDirection)
{

}