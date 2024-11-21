#include "pch.h"
#include "BoomerangWeapon.h"
#include "Mesh.h"
#include "MeshRenderer.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Animation.h"
#include "Animator.h"
#include "RectCollider.h"
#include "AttackDisappearBehavior.h"

BoomerangWeapon::BoomerangWeapon(UserType _userType)
{
	damage = 20.f;
	MeshRenderer* meshRenderer = new MeshRenderer();
	auto material = GGameManager->GetResourceManager()->Get<Material>(L"Default");
	meshRenderer->SetMaterial(material);
	meshRenderer->SetRenderingOrder(3);
	auto mesh = GAME->GetResourceManager()->Get<Mesh>(L"Midst1x1");
	meshRenderer->SetMesh(mesh);
	AddComponent(meshRenderer);

	Animator* animator = new Animator();
	auto animation = GAME->GetResourceManager()->Get<Animation>(L"boomerangWeaponLeft");
	animator->AddAnimation(0, animation);
	animation = GAME->GetResourceManager()->Get<Animation>(L"boomerangWeaponRight");
	animator->AddAnimation(1, animation);
	AddComponent(animator);

	Collider* weaponCollider = new RectCollider("BoomerangWeapon");
	userType = _userType;
	//if (userType == UserType::Kirby)
	{
		weaponCollider->AddCheckingTag("MonsterBasic");
		weaponCollider->AddCheckingTag("BossMonster");
	}
	//else
	{
		weaponCollider->AddCheckingTag("KirbyBasic");
	}
	weaponCollider->SetCollisionBehavior(new AttackDisappearBehavior(L"starExplosion"));
	vector<Vec3> vertices(5);
	vertices[0] = Vec3(-0.4f, 0.2f, 0.f);
	vertices[1] = Vec3(0.4f, 0.2f, 0.f);
	vertices[2] = Vec3(0.4f, -0.2f, 0.f);
	vertices[3] = Vec3(-0.4f, -0.2f, 0.f);
	vertices[4] = Vec3(-0.4f, 0.2f, 0.f);
	weaponCollider->SetVertices(vertices);
	AddComponent(weaponCollider);
}

BoomerangWeapon::~BoomerangWeapon()
{

}

void BoomerangWeapon::Trigger(const queue<KEY_TYPE>& keyQueue, const Vec3 summonerPosition, const Direction direction, GameObject* summoner)
{
	attackDirection = direction;

	if (attackDirection == Direction::RIGHT)
	{
		GetAnimator()->SetPresentAnimation(1);
	}
	else
	{
		GetAnimator()->SetPresentAnimation(0);
	}
	GetTransform()->SetPosition(summonerPosition + Vec3{ 0.f, 0.5f, 0.f });
}

void BoomerangWeapon::UpdateObject()
{
	Vec3 presentPosition = GetTransform()->GetPosition();
	Vec3 kirbyPosition = GAME->GetSceneManager()->GetKirbyPosition();

	float distance = abs(kirbyPosition.x - presentPosition.x);
	if (distance > 15.f) StopAttack(true);

	attackTime += Time->GetDeltaTime() * 15;
	float velocityX = 0.f;
	if (attackDirection == Direction::LEFT)
	{
		velocityX = attackTime * (1.f / 2.f) - 8.f;
	}
	else
	{
		velocityX = attackTime * (-1.f / 2.f) + 8.f;
	}

	Vec3 v = Vec3{ velocityX, 0.f, 0.f };
	GetTransform()->SetPosition(presentPosition + v * Time->GetDeltaTime());
}