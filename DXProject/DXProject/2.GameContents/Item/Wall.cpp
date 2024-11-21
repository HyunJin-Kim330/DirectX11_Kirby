#include "pch.h"
#include "Wall.h"
#include "RectCollider.h"
#include "MeshRenderer.h"
#include "Animator.h"
#include "Animation.h"

Wall::Wall(Vec3 wallPosition)
{
	GetTransform()->SetPosition(wallPosition);
	Collider* collider = new RectCollider("RectGrassGround");
	vector<Vec3> vertices(5);
	vertices[0] = Vec3(-0.4f, 1.8f, 0.f);
	vertices[1] = Vec3(0.4f, 1.8f, 0.f);
	vertices[2] = Vec3(0.4f, 0.f, 0.f);
	vertices[3] = Vec3(-0.4f, 0.f, 0.f);
	vertices[4] = Vec3(-0.4f, 1.8f, 0.f);

	collider->SetVertices(vertices);
	AddComponent(collider);

	MeshRenderer* meshRenderer = new MeshRenderer();
	AddComponent(meshRenderer);
	meshRenderer->SetRenderingOrder(1);
	auto mesh = GGameManager->GetResourceManager()->Get<Mesh>(L"Bottom1x2.3");
	meshRenderer->SetMesh(mesh);
	auto material = GGameManager->GetResourceManager()->Get<Material>(L"Default");
	meshRenderer->SetMaterial(material);

	Animator* animator = new Animator();
	AddComponent(animator);
	auto animation = GAME->GetResourceManager()->Get<Animation>(L"wallUp");
	animator->AddAnimation(0, animation);
	animation = GAME->GetResourceManager()->Get<Animation>(L"wallDown");
	animator->SetPresentAnimation(0);
	//animator->InactivateAnimator();
}

Wall::~Wall()
{

}

void Wall::Init()
{
	vector<Vec3> vertices(5);
	vertices[0] = Vec3(-0.4f, 1.8f, 0.f);
	vertices[1] = Vec3(0.4f, 1.8f, 0.f);
	vertices[2] = Vec3(0.4f, 0.f, 0.f);
	vertices[3] = Vec3(-0.4f, 0.f, 0.f);
	vertices[4] = Vec3(-0.4f, 1.8f, 0.f);

	GetCollider()->SetVertices(vertices);

	GetAnimator()->SetPresentAnimation(0);

	down = true;
	endOfTransform = true;
	GetAnimator()->ActivateAnimator();
}

void Wall::UpdateObject()
{
	if (!endOfTransform)
	{
		if (down)
		{
			GoDown();
		}
		else
		{
			GoUp();
		}
	}
	else
	{ 
		GetAnimator()->InactivateAnimator();
	}
}

void Wall::GoDown()
{
	GetAnimator()->SetPresentAnimation(1);
	GetAnimator()->ActivateAnimator();

	Collider* wallCollider = GetCollider();
	vector<Vec3> vertices = wallCollider->GetOriginalVertices();

	vertices[2] += Vec3{ 0.f, -0.02f, 0.f };
	vertices[3] += Vec3{ 0.f, -0.02f, 0.f };

	wallCollider->SetVertices(vertices);

	if (fabsf(vertices[2].y - vertices[1].y) > 1.8f)
	{
		endOfTransform = true;
	}
}

void Wall::GoUp()
{
	GetAnimator()->SetPresentAnimation(0);
	GetAnimator()->ActivateAnimator();

	Collider* wallCollider = GetCollider();
	vector<Vec3> vertices = wallCollider->GetOriginalVertices();

	vertices[2] += Vec3{ 0.f, 0.02f, 0.f };
	vertices[3] += Vec3{ 0.f, 0.02f, 0.f };

	wallCollider->SetVertices(vertices);

	if (fabsf(vertices[2].y - vertices[1].y) < 0.2f)
	{
		endOfTransform = true;
	}
}