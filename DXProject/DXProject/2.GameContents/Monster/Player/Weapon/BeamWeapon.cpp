#include "pch.h"
#include "AttackElectricShock.h"
#include "Animation.h"
#include "Animator.h"
#include "BeamWeapon.h"
#include "Mesh.h"
#include "MeshRenderer.h"
#include "LineCollider.h"
#include "SceneManager.h"
#include "Scene.h"

BeamWeapon::BeamWeapon(UserType userType)
{
	damage = 20.f;
	user = userType;

	MeshRenderer* meshRenderer = new MeshRenderer();
	auto material = GGameManager->GetResourceManager()->Get<Material>(L"Default");
	meshRenderer->SetMaterial(material);
	meshRenderer->SetRenderingOrder(3);
	AddComponent(meshRenderer);

	Animator* animator = new Animator();
	AddComponent(animator);

	if (userType == UserType::Kirby)
	{
		auto animation = GAME->GetResourceManager()->Get<Animation>(L"beamWeaponLeft");
		animator->AddAnimation(0, animation);
		animation = GAME->GetResourceManager()->Get<Animation>(L"beamWeaponRight");
		animator->AddAnimation(1, animation);
	}
	else
	{
		auto animation = GAME->GetResourceManager()->Get<Animation>(L"monster_beamWeaponLeft");
		animator->AddAnimation(0, animation);
		animation = GAME->GetResourceManager()->Get<Animation>(L"monster_beamWeaponRight");
		animator->AddAnimation(1, animation);
	}
	
	Collider* weaponCollider = new LineCollider("BeamWeapon");
	if (userType == UserType::Kirby)
	{
		weaponCollider->AddCheckingTag("MonsterBasic");
		weaponCollider->AddCheckingTag("BossMonster");
	}
	else
	{
		weaponCollider->AddCheckingTag("KirbyBasic");
	}
	weaponCollider->SetCollisionBehavior(new AttackElectricShock());
	AddComponent(weaponCollider);
}
	
BeamWeapon::~BeamWeapon()
{

}
	
void BeamWeapon::Trigger(const queue<KEY_TYPE>& keyQueue, const Vec3 summonerPosition, const Direction direction, GameObject* summoner)
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
		angle = 45;
		GetTransform()->SetPosition(summonerPosition + Vec3{ 0.2f, 0.4f, 0.f });

		if (user == UserType::Kirby)
		{
			auto mesh = GAME->GetResourceManager()->Get<Mesh>(L"Left4x4");
			GetMeshRenderer()->SetMesh(mesh);
		}
		else
		{
			auto mesh = GAME->GetResourceManager()->Get<Mesh>(L"Left_2x3.12");
			GetMeshRenderer()->SetMesh(mesh);
		}
		GetAnimator()->SetPresentAnimation(1);
		GetAnimator()->InitFrameNum();
	}
	else
	{
		angle = 135;
		GetTransform()->SetPosition(summonerPosition + Vec3{ -0.2f, 0.4f, 0.f });

		if (user == UserType::Kirby)
		{
			auto mesh = GAME->GetResourceManager()->Get<Mesh>(L"Right4x4");
			GetMeshRenderer()->SetMesh(mesh);
		}
		else
		{
			auto mesh = GAME->GetResourceManager()->Get<Mesh>(L"Right_2x3.12");
			GetMeshRenderer()->SetMesh(mesh);
		}
		GetAnimator()->SetPresentAnimation(0);
		GetAnimator()->InitFrameNum();
	}

	vector<Vec3> vertices(2);
	float x = cosf(angle * (3.141592 / 180)) * 3.f;
	float y = sinf(angle * (3.141592 / 180)) * 3.f;
	vertices[0] = Vec3(0.f, 0.f, 0.f);
	vertices[1] = Vec3(x, y, 0.f);
	GetCollider()->SetVertices(vertices);
}

void BeamWeapon::UpdateObject()
{
	attackTime += Time->GetDeltaTime();

	Collider* weaponCollider = GetCollider();
	vector<Vec3> vertices = weaponCollider->GetOriginalVertices();

	if (attackDirection == Direction::RIGHT)
	{
		if (user == UserType::Kirby) angle -= 3.f;
		else angle -= 1.f;
	}
	else
	{
		if (user == UserType::Kirby) angle += 3.f;
		else angle += 1.f;
	}

	float x = cosf(angle * (3.141592 / 180)) * 3.f;
	float y = sinf(angle * (3.141592 / 180)) * 3.f;
	vertices[1] = Vec3(x, y, 0.f);
	weaponCollider->SetVertices(vertices);

	if (user == UserType::Kirby && attackTime >= 0.5f)
	{
		StopAttack();
	}

	if (user == UserType::Monster && attackTime >= 0.9f)
	{
		StopAttack();
	}
}