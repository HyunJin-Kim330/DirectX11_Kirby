#include "pch.h"
#include "BreathWeapon.h"
#include "Collider.h"
#include "RectCollider.h"
#include "AttackDisappearBehavior.h"
#include "Mesh.h"
#include "MeshRenderer.h"
#include "Animator.h"
#include "Animation.h"

BreathWeapon::BreathWeapon()
{
	damage = 10.f;
	/*MeshRenderer* meshRenderer = new MeshRenderer();
	AddComponent(meshRenderer);
	Animator* animator = new Animator();
	AddComponent(animator);*/
}

BreathWeapon::~BreathWeapon()
{

}

void BreathWeapon::UpdateObject()
{
	BreathAttack();
}

void BreathWeapon::Trigger(const queue<KEY_TYPE>& keyQueue, const Vec3 summonerPosition, const Direction direction, GameObject* summoner)
{
	attackDirection = direction;

	MeshRenderer* meshRenderer = new MeshRenderer();
	AddComponent(meshRenderer);
	meshRenderer->SetRenderingOrder(3);
	Animator* animator = new Animator();
	AddComponent(animator);

	Collider* weaponCollider = new RectCollider("BreathWeapon");
	weaponCollider->AddCheckingTag("MonsterBasic");
	weaponCollider->AddCheckingTag("StarBlock");
	weaponCollider->SetCollisionBehavior(new AttackDisappearBehavior());  // Breath weapon'll disappear when collide
	
	if (attackDirection == Direction::RIGHT)
	{
		GetTransform()->SetPosition(summonerPosition + Vec3{ 0.2f, 0.4f, 0.f });
		vector<Vec3> vertices(5);

		auto mesh = GAME->GetResourceManager()->Get<Mesh>(L"Left2.8x0.9");
		meshRenderer->SetMesh(mesh);
		auto material = GGameManager->GetResourceManager()->Get<Material>(L"Default");
		meshRenderer->SetMaterial(material);

		auto animation = GAME->GetResourceManager()->Get<Animation>(L"breathWeaponRight");
		animator->AddAnimation(0, animation);
		animator->SetPresentAnimation(0);

		vertices[0] = Vec3(0.f, 0.4f, 0.f);
		vertices[1] = Vec3(0.8f, 0.4f, 0.f);
		vertices[2] = Vec3(0.8f, -0.4f, 0.f);
		vertices[3] = Vec3(0.f, -0.4f, 0.f);
		vertices[4] = Vec3(0.f, 0.4f, 0.f);
		weaponCollider->SetVertices(vertices);
	}
	else
	{
		GetTransform()->SetPosition(summonerPosition + Vec3{ -0.2f, 0.4f, 0.f });
		vector<Vec3> vertices(5);

		auto mesh = GAME->GetResourceManager()->Get<Mesh>(L"Right2.8x0.9");
		meshRenderer->SetMesh(mesh);
		auto material = GGameManager->GetResourceManager()->Get<Material>(L"Default");
		meshRenderer->SetMaterial(material);

		auto animation = GAME->GetResourceManager()->Get<Animation>(L"breathWeaponLeft");
		animator->AddAnimation(0, animation);
		animator->SetPresentAnimation(0);

		vertices[0] = Vec3(-0.8f, 0.4f, 0.f);
		vertices[1] = Vec3(0.f, 0.4f, 0.f);
		vertices[2] = Vec3(0.f, -0.4f, 0.f);
		vertices[3] = Vec3(-0.8f, -0.4f, 0.f);
		vertices[4] = Vec3(-0.8f, 0.4f, 0.f);
		weaponCollider->SetVertices(vertices);
	}
	this->AddComponent(weaponCollider);
}

void BreathWeapon::BreathAttack()
{
	attackTime += Time->GetDeltaTime();
	colliderUpdateTime += Time->GetDeltaTime();

	if (colliderUpdateTime <= 0.1f)
	{
		return;
	}
	else
	{
		colliderUpdateTime = 0.f;
	}

	
	if (attackDirection == Direction::RIGHT)
	{
		Collider* weaponCollider = GetCollider();
		vector<Vec3> vertices = weaponCollider->GetOriginalVertices();

		vertices[1] += Vec3{ 0.4f, 0.f, 0.f };
		vertices[2] += Vec3{ 0.4f, 0.f, 0.f };
		
		weaponCollider->SetVertices(vertices);
	}
	else
	{
		Collider* weaponCollider = GetCollider();
		vector<Vec3> vertices = weaponCollider->GetOriginalVertices();

		vertices[0] += Vec3{ -0.4f, 0.f, 0.f };
		vertices[4] += Vec3{ -0.4f, 0.f, 0.f };
		vertices[3] += Vec3{ -0.4f, 0.f, 0.f };

		weaponCollider->SetVertices(vertices);
	}

	if (attackTime >= 0.6f)
	{
		StopAttack(true);
	}
}
