#include "pch.h"
#include "IceBlock.h"
#include "RectCollider.h"
#include "Animator.h"
#include "Animation.h"
#include "AttackDisappearBehavior.h"
#include "MeshRenderer.h"
#include "SceneManager.h"
#include "Scene.h"

IceBlock::IceBlock(Vec3 position)
{
	GetTransform()->SetPosition(position);

	Collider* collider = new RectCollider("NotMoveStar");
	AddComponent(collider);
	collider->SetCollisionBehavior(new AttackDisappearBehavior(L"starExplosion"));

	vector<Vec3> vertices(5);
	vertices[0] = Vec3(-0.4f, 0.4f, 0.f);
	vertices[1] = Vec3(0.4f, 0.4f, 0.f);
	vertices[2] = Vec3(0.4f, -0.4f, 0.f);
	vertices[3] = Vec3(-0.4f, -0.4f, 0.f);
	vertices[4] = Vec3(-0.4f, 0.4f, 0.f);
	collider->SetVertices(vertices);

	MeshRenderer* meshRenderer = new MeshRenderer();
	AddComponent(meshRenderer);
	auto mesh = GAME->GetResourceManager()->Get<Mesh>(L"Midst1x1");
	meshRenderer->SetMesh(mesh);
	auto material = GAME->GetResourceManager()->Get<Material>(L"Default");
	meshRenderer->SetMaterial(material);
	meshRenderer->SetRenderingOrder(3);

	Animator* animator = new Animator();
	AddComponent(animator);
	auto animation = GAME->GetResourceManager()->Get<Animation>(L"iceBlock");
	animator->AddAnimation(0, animation);
	animator->SetPresentAnimation(0);
}

IceBlock::~IceBlock()
{

}

void IceBlock::UpdateObject()
{
	Vec3 kirbyPosition = GAME->GetSceneManager()->GetKirbyPosition();
	Vec3 presentPosition = GetTransform()->GetPosition();
	float distance = fabsf(kirbyPosition.x - presentPosition.x);

	if (distance > 15.f)
	{
		Disappear();
	}
	else if (distance <= 0.6f)
	{
		velocity.x = kirbyPosition.x - presentPosition.x < 0.f ? 14.5f : -14.5f;
	}

	if (lifeTimeCalc > lifeTime)
	{
		AddGameObjectToScene(Pool->GetEffectObject(L"starExplosion", GetMidstPosition()));
		Disappear();
	}
	else
	{
		lifeTimeCalc += Time->GetDeltaTime();
	}

	GetTransform()->SetPosition(presentPosition + velocity * Time->GetDeltaTime());
}

void IceBlock::Disappear()
{
	GAME->GetSceneManager()->GetActiveScene()->RemoveGameObject(this);
}