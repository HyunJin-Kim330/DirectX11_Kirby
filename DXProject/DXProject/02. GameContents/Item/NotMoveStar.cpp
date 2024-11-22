#include "pch.h"
#include "02. GameContents/Item/NotMoveStar.h"
#include "00. Engine/Component/Animator/Animator.h"
#include "00. Engine/Manager/ResourceManager/Resource/Animation.h"
#include "00. Engine/Component/Collider/RectCollider.h"
#include "00. Engine/Manager/SceneManager/SceneManager.h"
#include "02. GameContents/Item/MakeStarBehavior.h"
#include "00. Engine/Component/MeshRenderer/MeshRenderer.h"

NotMoveStar::NotMoveStar(Vec3 position)
{
	GetTransform()->SetPosition(position);

	Collider* collider = new RectCollider("NotMoveStar");
	AddComponent(collider);
	collider->AddCheckingTag("KirbyBasic");
	collider->SetCollisionBehavior(new MakeStarBehavior());

	vector<Vec3> vertices(5);
	vertices[0] = Vec3(-0.4f, 0.8f, 0.f);
	vertices[1] = Vec3(0.4f, 0.8f, 0.f);
	vertices[2] = Vec3(0.4f, 0.f, 0.f);
	vertices[3] = Vec3(-0.4f, 0.f, 0.f);
	vertices[4] = Vec3(-0.4f, 0.8f, 0.f);
	collider->SetVertices(vertices);

	MeshRenderer* meshRenderer = new MeshRenderer();
	AddComponent(meshRenderer);
	auto mesh = GAME->GetResourceManager()->Get<Mesh>(L"Bottom1.3x1.3");
	meshRenderer->SetMesh(mesh);
	auto material = GAME->GetResourceManager()->Get<Material>(L"Default");
	meshRenderer->SetMaterial(material);
	meshRenderer->SetRenderingOrder(3);

	Animator* animator = new Animator();
	AddComponent(animator);
	auto animation = GAME->GetResourceManager()->Get<Animation>(L"notMoveStar");
	animator->AddAnimation(0, animation);
	animator->SetPresentAnimation(0);		// 현재 상태에 맞게 애니메이션 셋팅
}

NotMoveStar::~NotMoveStar()
{

}

void NotMoveStar::UpdateObject()
{
	if (sucked)
	{
		Vec3 presentPosition = GetTransform()->GetPosition();
		GetTransform()->SetPosition(presentPosition + velocity * Time->GetDeltaTime());
	}

	if (lifeTimeCalc > lifeTime)
	{
		if(!sucked)	Disappear();
	}
	else
	{
		lifeTimeCalc += Time->GetDeltaTime();
	}
}

void NotMoveStar::AlertSucked(Vec3 _suckedPosition)
{
	sucked = true;
	suckedPosition = _suckedPosition;

	Vec3 position = GetTransform()->GetPosition();
	velocity.x = (suckedPosition.x - position.x) * 4.5f;
	velocity.y = (suckedPosition.y - position.y) * 4.5f;
}

void NotMoveStar::Disappear()
{
	GAME->GetSceneManager()->GetActiveScene()->RemoveGameObject(this);
}