#include "pch.h"
#include "AbilityStar.h"
#include "AbilityStarCollisionBehavior.h"
#include "Animator.h"
#include "Animation.h"
#include "RectCollider.h"
#include "Scene.h"
#include "SceneManager.h"
#include "Mesh.h"
#include "MeshRenderer.h"

AbilityStar::AbilityStar(Ability* _ability, Direction _direction)
{
	ability = _ability;
	velocity.x = _direction == Direction::LEFT ? -2.f : 2.f;
	GetTransform()->SetPosition(GAME->GetSceneManager()->GetKirbyPosition());

	Collider* collider = new RectCollider("AbilityStar");
	AddComponent(collider);
	collider->AddCheckingTag("KirbyBasic");
	collider->AddCheckingTag("RectGrassGround");
	collider->SetCollisionBehavior(new AbilityStarCollisionBehavior());

	vector<Vec3> vertices(5);
	vertices[0] = Vec3(-0.3f, 0.6f, 0.f);
	vertices[1] = Vec3(0.3f, 0.6f, 0.f);
	vertices[2] = Vec3(0.3f, 0.f, 0.f);
	vertices[3] = Vec3(-0.3f, 0.f, 0.f);
	vertices[4] = Vec3(-0.3f, 0.6f, 0.f);
	collider->SetVertices(vertices);

	MeshRenderer* meshRenderer = new MeshRenderer();
	AddComponent(meshRenderer);
	auto mesh = GAME->GetResourceManager()->Get<Mesh>(L"Bottom1x1");
	meshRenderer->SetMesh(mesh);
	auto material = GAME->GetResourceManager()->Get<Material>(L"Default");
	meshRenderer->SetMaterial(material);
	meshRenderer->SetRenderingOrder(2);

	Animator* animator = new Animator();
	AddComponent(animator);
	auto animation = GAME->GetResourceManager()->Get<Animation>(L"abilityStar");
	animator->AddAnimation(0, animation);
	animator->SetPresentAnimation(0);		// 현재 상태에 맞게 애니메이션 셋팅
}

AbilityStar::~AbilityStar()
{

}

Ability* AbilityStar::GetAbility()
{
	return ability;
}

void AbilityStar::UpdateObject()
{
	if (sucked)
	{
		Sucked();
	}
	else
	{
		disappearTimeCalc += Time->GetDeltaTime();
		Jump();
	}

	if (disappearTimeCalc >= disappearTime)
	{
		Disappear(true);
	}

	Vec3 v = Vec3{ velocity.x, -1.f * 2.f + velocity.y, velocity.z };
	Vec3 presentPosition = GetTransform()->GetPosition();
	GetTransform()->SetPosition(presentPosition + v * Time->GetDeltaTime());
}

void AbilityStar::Sucked()
{
	Vec3 monsterPosition = GetTransform()->GetPosition();
	velocity.x = (suckedPosition.x - monsterPosition.x) * 4.5f;
	velocity.y = (suckedPosition.y - monsterPosition.y) * 4.5f;
}

void AbilityStar::Jump()
{
	jumpTimeCalc += Time->GetDeltaTime() * 15;
	velocity.y = jumpTimeCalc * (-1.f / 2) + jumpPower;
}

void AbilityStar::AlertSucked(Vec3 _suckedPosition)
{
	ChangeSucked();
	suckedPosition = _suckedPosition;
}

void AbilityStar::AlertLandGround()
{
	jumpTimeCalc = 0.f;
	velocity.y = 0.f;
}

void AbilityStar::AlertCollideSideGround(Direction collideDirection)
{
	velocity.x = collideDirection == Direction::LEFT ? 2.f : -2.f;
}

void AbilityStar::Disappear(bool useEffect)
{

	GAME->GetSceneManager()->GetActiveScene()->RemoveGameObject(this);

	if (useEffect)
	{
		GAME->GetSceneManager()->GetActiveScene()->AddGameObject(Pool->GetEffectObject(L"monsterExplosionBasic", GetMidstPosition()));
	}
}