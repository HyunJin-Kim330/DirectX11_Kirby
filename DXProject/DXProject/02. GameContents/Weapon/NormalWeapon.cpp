#include "pch.h"
#include "02. GameContents/Weapon/NormalWeapon.h"
#include "00. Engine/Component/Collider/Collider.h"
#include "00. Engine/Component/Collider/RectCollider.h"
#include "02. GameContents/Weapon/CollisionBehavior/SuckBehavior.h"
#include "00. Engine/Manager/ResourceManager/Resource/Mesh.h"
#include "00. Engine/Component/MeshRenderer/MeshRenderer.h"
#include "00. Engine/Component/Animator/Animator.h"
#include "00. Engine/Manager/ResourceManager/Resource/Animation.h"

NormalWeapon::NormalWeapon()
{
	MeshRenderer* meshRenderer = new MeshRenderer();
	auto material = GGameManager->GetResourceManager()->Get<Material>(L"Default");
	meshRenderer->SetMaterial(material);
	meshRenderer->SetRenderingOrder(3);
	AddComponent(meshRenderer);

	Animator* animator = new Animator();
	AddComponent(animator);

	auto animation = GAME->GetResourceManager()->Get<Animation>(L"normalWeaponLeft");
	animator->AddAnimation(0, animation);
	animation = GAME->GetResourceManager()->Get<Animation>(L"normalWeaponRight");
	animator->AddAnimation(1, animation);

}

NormalWeapon::~NormalWeapon()
{

}

void NormalWeapon::UpdateObject()
{
	SuckAttack();
}

void NormalWeapon::Trigger(const queue<KEY_TYPE>& keyQueue, const Vec3 _summonerPosition, const Direction direction, GameObject* summoner)
{
	attackDirection = direction;
	summonerPosition = _summonerPosition;

	SetInitialPosition();
	SetInitialCollider();

	if (GetMeshRenderer() != nullptr)
	{
		GetMeshRenderer()->ActiveMeshRenderer();
	}

	if (attackDirection == Direction::RIGHT)
	{
		GetTransform()->SetPosition(summonerPosition + Vec3{ 0.2f, 0.4f, 0.f });

		auto mesh = GAME->GetResourceManager()->Get<Mesh>(L"Left4x2");
		GetMeshRenderer()->SetMesh(mesh);

		GetAnimator()->SetPresentAnimation(1);
		GetAnimator()->InitFrameNum();
	}
	else
	{
		GetTransform()->SetPosition(summonerPosition + Vec3{ -0.2f, 0.4f, 0.f });

		auto mesh = GAME->GetResourceManager()->Get<Mesh>(L"Right4x2");
		GetMeshRenderer()->SetMesh(mesh);

		GetAnimator()->SetPresentAnimation(0);
		GetAnimator()->InitFrameNum();
	}
}

void NormalWeapon::SuckAttack()
{
	attackTime += Time->GetDeltaTime();

	if (0.75f <= attackTime && attackTime < 1.5f)
	{
		Collider* weaponCollider = GetCollider();
		vector<Vec3> vertices(5);

		if (attackDirection == Direction::RIGHT)
		{
			vertices[0] = Vec3(0.f, 1.f, 0.f);
			vertices[1] = Vec3(3.2f, 1.f, 0.f);
			vertices[2] = Vec3(3.2f, -1.f, 0.f);
			vertices[3] = Vec3(0.f, -1.f, 0.f);
			vertices[4] = Vec3(0.f, 1.f, 0.f);
		}
		else
		{
			vertices[0] = Vec3(-3.2f, 1.f, 0.f);
			vertices[1] = Vec3(0.f, 1.f, 0.f);
			vertices[2] = Vec3(0.f, -1.f, 0.f);
			vertices[3] = Vec3(-3.2f, -1.f, 0.f);
			vertices[4] = Vec3(-3.2f, 1.f, 0.f);
		}
		
		weaponCollider->SetVertices(vertices);
	}

	if (1.5f <= attackTime)
	{
		
	}

	if (1.58f <= attackTime)
	{
		StopAttack();
	}
}

void NormalWeapon::SetInitialCollider()
{
	Collider* weaponCollider = new RectCollider("NormalWeapon");
	weaponCollider->AddCheckingTag("MonsterBasic");
	weaponCollider->AddCheckingTag("AbilityStar");
	weaponCollider->AddCheckingTag("StarBlock");
	weaponCollider->AddCheckingTag("NotMoveStar");
	weaponCollider->AddCheckingTag("BossMonster");
	weaponCollider->SetCollisionBehavior(new SuckBehavior());
	vector<Vec3> vertices(5);

	if (attackDirection == Direction::RIGHT)
	{
		vertices[0] = Vec3(0.f, 0.8f, 0.f);
		vertices[1] = Vec3(3.0f, 0.8f, 0.f);
		vertices[2] = Vec3(3.0f, -0.8f, 0.f);
		vertices[3] = Vec3(0.f, -0.8f, 0.f);
		vertices[4] = Vec3(0.f, 0.8f, 0.f);
	}
	else
	{
		vertices[0] = Vec3(-3.0f, 0.8f, 0.f);
		vertices[1] = Vec3(0.f, 0.8f, 0.f);
		vertices[2] = Vec3(0.f, -0.8f, 0.f);
		vertices[3] = Vec3(-3.0f, -0.8f, 0.f);
		vertices[4] = Vec3(-3.0f, 0.8f, 0.f);
	}
	
	weaponCollider->SetVertices(vertices);
	this->AddComponent(weaponCollider);
}

void NormalWeapon::SetInitialPosition()
{
	if (attackDirection == Direction::RIGHT)
	{
		GetTransform()->SetPosition(summonerPosition + Vec3{ 0.2f, 0.4f, 0.f });
	}
	else
	{
		GetTransform()->SetPosition(summonerPosition + Vec3{ -0.2f, 0.4f, 0.f });
	}
}