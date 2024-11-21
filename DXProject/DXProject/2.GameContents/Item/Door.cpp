#include "pch.h"
#include "Animation.h"
#include "Animator.h"
#include "Door.h"
#include "Mesh.h"
#include "MeshRenderer.h"
#include "RectCollider.h"

Door::Door(string _sceneName, Vec3 _kirbyInitPosition, Vec3 _cameraInitPosition)
{
	sceneName = _sceneName;
	kirbyInitPosition = _kirbyInitPosition;
	cameraInitPosition = _cameraInitPosition;

	MeshRenderer* meshRenderer = new MeshRenderer();
	AddComponent(meshRenderer);
	meshRenderer->SetRenderingOrder(2);
	auto mesh = GGameManager->GetResourceManager()->Get<Mesh>(L"Bottom1.6x1.6");
	meshRenderer->SetMesh(mesh);
	auto material = GGameManager->GetResourceManager()->Get<Material>(L"Default");
	meshRenderer->SetMaterial(material);

	Animator* animator = new Animator();
	AddComponent(animator);
	auto animation = GGameManager->GetResourceManager()->Get<Animation>(L"door");
	animator->AddAnimation(0, animation);
	animator->SetPresentAnimation(0);

	Collider* collider = new RectCollider("Door");
	vector<Vec3> vertices(5);

	vertices[0] = Vec3(-0.3f, 0.8f, 0.f);
	vertices[1] = Vec3(0.3f, 0.8f, 0.f);
	vertices[2] = Vec3(0.3f, 0.f, 0.f);
	vertices[3] = Vec3(-0.3f, 0.f, 0.f);
	vertices[4] = Vec3(-0.3f, 0.8f, 0.f);

	collider->SetVertices(vertices);
	AddComponent(collider);
}

Door::~Door()
{
	
}

void Door::UpdateObject()
{

}
