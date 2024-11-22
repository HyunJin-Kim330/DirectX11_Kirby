#include "pch.h"
#include "02. GameContents/Weapon/StarWeapon.h"
#include "00. Engine/Manager/ResourceManager/Resource/Mesh.h"
#include "00. Engine/Component/MeshRenderer/MeshRenderer.h"
#include "00. Engine/Component/Animator/Animator.h"
#include "00. Engine/Manager/ResourceManager/Resource/Animation.h"
#include "02. GameContents/Weapon/CollisionBehavior/AttackDisappearBehavior.h"
#include "00. Engine/Component/Collider/RectCollider.h"

StarWeapon::StarWeapon()
{
	damage = 10.f;
}

StarWeapon::~StarWeapon()
{

}

void StarWeapon::Trigger(const queue<KEY_TYPE>& keyQueue, const Vec3 summonerPosition, const Direction direction, GameObject* summoner)
{
	attackDirection = direction;

	MeshRenderer* meshRenderer = new MeshRenderer();
	AddComponent(meshRenderer);
	Animator* animator = new Animator();
	AddComponent(animator);

	auto mesh = GAME->GetResourceManager()->Get<Mesh>(L"Midst1x1");
	meshRenderer->SetMesh(mesh);
	auto material = GGameManager->GetResourceManager()->Get<Material>(L"Default");
	meshRenderer->SetMaterial(material);
	meshRenderer->SetRenderingOrder(3);

	if (attackDirection == Direction::RIGHT)
	{
		GetTransform()->SetPosition(summonerPosition + Vec3{ 0.2f, 0.5f, 0.f });
		auto animation = GAME->GetResourceManager()->Get<Animation>(L"starWeaponRight");
		animator->AddAnimation(0, animation);
		animator->SetPresentAnimation(0);
	}
	else
	{
		GetTransform()->SetPosition(summonerPosition + Vec3{ -0.2f, 0.5f, 0.f });
		auto animation = GAME->GetResourceManager()->Get<Animation>(L"starWeaponLeft");
		animator->AddAnimation(0, animation);
		animator->SetPresentAnimation(0);
	}

	Collider* weaponCollider = new RectCollider("StarWeapon");
	weaponCollider->AddCheckingTag("MonsterBasic");
	weaponCollider->AddCheckingTag("RectGrassGround");
	weaponCollider->AddCheckingTag("StarBlock");
	weaponCollider->AddCheckingTag("BossMonster");
	weaponCollider->SetCollisionBehavior(new AttackDisappearBehavior(L"starExplosion"));

	vector<Vec3> vertices(5);
	vertices[0] = Vec3(-0.4f, 0.4f, 0.f);
	vertices[1] = Vec3(0.4f, 0.4f, 0.f);
	vertices[2] = Vec3(0.4f, -0.4f, 0.f);
	vertices[3] = Vec3(-0.4f, -0.4f, 0.f);
	vertices[4] = Vec3(-0.4f, 0.4f, 0.f);
	weaponCollider->SetVertices(vertices);
	
	AddComponent(weaponCollider);
}

void StarWeapon::UpdateObject()
{
	Vec3 presentPosition = GetTransform()->GetPosition();

	if (attackDirection == Direction::RIGHT)
	{
		GetTransform()->SetPosition(presentPosition + Vec3{ 8.f, 0.f, 0.f } * Time->GetDeltaTime());
	}
	else
	{
		GetTransform()->SetPosition(presentPosition + Vec3{ -8.f, 0.f, 0.f } *Time->GetDeltaTime());
	}
}