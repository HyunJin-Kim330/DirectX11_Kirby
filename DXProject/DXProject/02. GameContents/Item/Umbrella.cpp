#include "pch.h"
#include "00. Engine/Component/Animator/Animator.h"
#include "00. Engine/Manager/ResourceManager/Resource/Animation.h"
#include "00. Engine/Component/Animator/Animator.h"
#include "00. Engine/Component/Collider/RectCollider.h"
#include "02. GameContents/Item/Umbrella.h"
#include "00. Engine/Component/MeshRenderer/MeshRenderer.h"
#include "00. Engine/Manager/SceneManager/SceneManager.h"

Umbrella::Umbrella()
{
	MeshRenderer* meshRenderer = new MeshRenderer();
	AddComponent(meshRenderer);
	meshRenderer->SetRenderingOrder(2);
	auto mesh = GGameManager->GetResourceManager()->Get<Mesh>(L"Bottom1x1");
	meshRenderer->SetMesh(mesh);
	auto material = GGameManager->GetResourceManager()->Get<Material>(L"Default");
	meshRenderer->SetMaterial(material);

	Animator* animator = new Animator();
	AddComponent(animator);
	auto animation = GGameManager->GetResourceManager()->Get<Animation>(L"umbrellaDown");
	animator->AddAnimation(0, animation);
	animation = GGameManager->GetResourceManager()->Get<Animation>(L"umbrellaUp");
	animator->AddAnimation(1, animation);
	animator->SetPresentAnimation(0);

	Collider* collider = new RectCollider("Umbrella");
	vector<Vec3> vertices(5);

	vertices[0] = Vec3(-0.3f, 0.6f, 0.f);
	vertices[1] = Vec3(0.3f, 0.6f, 0.f);
	vertices[2] = Vec3(0.3f, 0.f, 0.f);
	vertices[3] = Vec3(-0.3f, 0.f, 0.f);
	vertices[4] = Vec3(-0.3f, 0.6f, 0.f);

	collider->SetVertices(vertices);
	AddComponent(collider);
}

Umbrella::~Umbrella()
{

}

void Umbrella::UpdateObject()
{
	Vec3 presentPosition = GetTransform()->GetPosition();
	GetTransform()->SetPosition(presentPosition + velocity *Time->GetDeltaTime());

	Vec3 kirbyPosition = GAME->GetSceneManager()->GetKirbyPosition();
	if (fabsf(presentPosition.y - kirbyPosition.y) > 15.f)
	{
		Disappear();
	}
}

void Umbrella::ReleaseFromMonster()
{ 
	if (sucked) return;
	
	atatchMonster = false; 
	GetAnimator()->SetPresentAnimation(1);
	velocity.y = 2.f;

	if (GetParent() != nullptr)
	{
		GetParent()->DeleteChild(this);
		parent = nullptr;
	}
}

void Umbrella::SetSuckedTrue(Vec3 suckedPosition)
{
	sucked = true;

	Vec3 presentPosition = GetTransform()->GetPosition();
	velocity.x = (suckedPosition.x - presentPosition.x) * 4.5f;
	velocity.y = (suckedPosition.y - presentPosition.y) * 4.5f;
}

void Umbrella::Disappear() 
{
	GAME->GetSceneManager()->GetActiveScene()->RemoveGameObject(this);
}