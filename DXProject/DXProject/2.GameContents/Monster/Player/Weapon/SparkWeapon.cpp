#include "pch.h"
#include "AttackElectricShock.h"
#include "Animation.h"
#include "Animator.h"
#include "SparkWeapon.h"
#include "Mesh.h"
#include "MeshRenderer.h"
#include "SceneManager.h"
#include "Scene.h"
#include "DotCollider.h"

SparkWeapon::SparkWeapon(UserType userType)
{
	damage = 20.f;

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
		auto animation = GAME->GetResourceManager()->Get<Animation>(L"sparkWeaponLeft");
		animator->AddAnimation(0, animation);
		animation = GAME->GetResourceManager()->Get<Animation>(L"sparkWeaponRight");
		animator->AddAnimation(1, animation);
	}
	else
	{
		auto animation = GAME->GetResourceManager()->Get<Animation>(L"monster_sparkWeaponLeft");
		animator->AddAnimation(0, animation);
		animation = GAME->GetResourceManager()->Get<Animation>(L"monster_sparkWeaponRight");
		animator->AddAnimation(1, animation);
	}
	AddComponent(animator);

	Collider* weaponCollider = new DotCollider("SparkWeapon", 1.f);
	if (userType == UserType::Kirby)
	{
		weaponCollider->AddCheckingTag("BossMonster");
		weaponCollider->AddCheckingTag("MonsterBasic");
	}
	else
	{
		weaponCollider->AddCheckingTag("KirbyBasic");
	}
	weaponCollider->SetCollisionBehavior(new AttackElectricShock());
	vector<Vec3> vertices(1);
	vertices[0] = Vec3(0.f, 0.f, 0.f);
	weaponCollider->SetVertices(vertices);
	AddComponent(weaponCollider);
}

SparkWeapon::~SparkWeapon()
{

}

void SparkWeapon::Trigger(const queue<KEY_TYPE>& keyQueue, const Vec3 summonerPosition, const Direction direction, GameObject* summoner)
{
	attackDirection = direction;

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

	GetTransform()->SetPosition(summonerPosition + Vec3{ 0.f, 0.4f, 0.f });
}

void SparkWeapon::UpdateObject()
{

}