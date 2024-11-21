#include "pch.h"
#include "Animation.h"
#include "Animator.h"
#include "AttackChangeIce.h"
#include "IceWeapon.h"
#include "MeshRenderer.h"
#include "RectCollider.h"
#include "SceneManager.h"
#include "Scene.h"

IceWeapon::IceWeapon(UserType userType)
{
	damage = 20.f;
	user = userType;

	MeshRenderer* meshRenderer = new MeshRenderer();
	auto material = GGameManager->GetResourceManager()->Get<Material>(L"Default");
	meshRenderer->SetMaterial(material);
	meshRenderer->SetRenderingOrder(3);
	auto mesh = GAME->GetResourceManager()->Get<Mesh>(L"Midst3x3");
	meshRenderer->SetMesh(mesh);
	AddComponent(meshRenderer);

	Animator* animator = new Animator();
	if (userType == UserType::Kirby)
	{
		auto animation = GAME->GetResourceManager()->Get<Animation>(L"iceWeaponLeft");
		animator->AddAnimation(0, animation);
		animation = GAME->GetResourceManager()->Get<Animation>(L"iceWeaponRight");
		animator->AddAnimation(1, animation);
	}
	else
	{
		auto animation = GAME->GetResourceManager()->Get<Animation>(L"monster_iceWeaponLeft");
		animator->AddAnimation(0, animation);
		animation = GAME->GetResourceManager()->Get<Animation>(L"monster_iceWeaponRight");
		animator->AddAnimation(1, animation);
	}
	AddComponent(animator);

	Collider* weaponCollider = new RectCollider("IceWeapon");
	if (userType == UserType::Kirby)
	{
		weaponCollider->AddCheckingTag("BossMonster");
		weaponCollider->AddCheckingTag("MonsterBasic");
	}
	else
	{
		weaponCollider->AddCheckingTag("KirbyBasic");
	}
	weaponCollider->SetCollisionBehavior(new AttackChangeIce());
	AddComponent(weaponCollider);

	vector<Vec3> vertices(1);
	vertices[0] = Vec3(0.f, 0.f, 0.f);
	weaponCollider->SetVertices(vertices);
}

IceWeapon::~IceWeapon()
{

}

void IceWeapon::Trigger(const queue<KEY_TYPE>& keyQueue, const Vec3 _summonerPosition, const Direction direction, GameObject* summoner)
{
	attackDirection = direction;
	summonerPosition = _summonerPosition;

	if (GetParent() == nullptr)
	{
		summoner->AddChild(this);
		SetParent(summoner);
	}

	if (GetMeshRenderer() != nullptr)
	{
		GetMeshRenderer()->ActiveMeshRenderer();
	}

	if (GetCollider() != nullptr)
	{
		GetCollider()->Activate();
	}

	if (attackDirection == Direction::RIGHT)
	{
		GetAnimator()->SetPresentAnimation(1);
		GetAnimator()->InitFrameNum();
	}
	else
	{
		GetAnimator()->SetPresentAnimation(0);
		GetAnimator()->InitFrameNum();
	}

	SetInitialPosition();
	SetInitialCollider();
}

void IceWeapon::UpdateObject()
{

}

void IceWeapon::SetInitialCollider()
{
	vector<Vec3> vertices(5);

	if (user == UserType::Kirby)
	{
		if (attackDirection == Direction::RIGHT)
		{
			vertices[0] = Vec3(-1.4f, 0.6f, 0.f);
			vertices[1] = Vec3(1.4f, 0.6f, 0.f);
			vertices[2] = Vec3(1.4f, -0.6f, 0.f);
			vertices[3] = Vec3(-1.4f, -0.6f, 0.f);
			vertices[4] = Vec3(-1.4f, 0.6f, 0.f);
		}
		else
		{
			vertices[0] = Vec3(-1.4f, 0.6f, 0.f);
			vertices[1] = Vec3(1.4f, 0.6f, 0.f);
			vertices[2] = Vec3(1.4f, -0.6f, 0.f);
			vertices[3] = Vec3(-1.4f, -0.6f, 0.f);
			vertices[4] = Vec3(-1.4f, 0.6f, 0.f);
		}
	}
	else
	{
		if (attackDirection == Direction::RIGHT)
		{
			vertices[0] = Vec3(-1.f, 0.6f, 0.f);
			vertices[1] = Vec3(1.f, 0.6f, 0.f);
			vertices[2] = Vec3(1.f, -0.6f, 0.f);
			vertices[3] = Vec3(-1.f, -0.6f, 0.f);
			vertices[4] = Vec3(-1.f, 0.6f, 0.f);
		}
		else
		{
			vertices[0] = Vec3(-1.f, 0.6f, 0.f);
			vertices[1] = Vec3(1.f, 0.6f, 0.f);
			vertices[2] = Vec3(1.f, -0.6f, 0.f);
			vertices[3] = Vec3(-1.f, -0.6f, 0.f);
			vertices[4] = Vec3(-1.f, 0.6f, 0.f);
		}
	}

	GetCollider()->SetVertices(vertices);
}

void IceWeapon::SetInitialPosition()
{
	if (user == UserType::Kirby)
	{
		if (attackDirection == Direction::RIGHT)
		{
			GetTransform()->SetPosition(summonerPosition + Vec3{ 2.5f, 0.6f, 0.f });
		}
		else
		{
			GetTransform()->SetPosition(summonerPosition + Vec3{ -2.5f, 0.6f, 0.f });
		}
	}
	else
	{
		if (attackDirection == Direction::RIGHT)
		{
			GetTransform()->SetPosition(summonerPosition + Vec3{ 2.f, 0.6f, 0.f });
		}
		else
		{
			GetTransform()->SetPosition(summonerPosition + Vec3{ -2.f, 0.6f, 0.f });
		}
	}
}