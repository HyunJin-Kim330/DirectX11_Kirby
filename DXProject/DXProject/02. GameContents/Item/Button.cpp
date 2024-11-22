#include "pch.h"
#include "00. Engine/Component/Animator/Animator.h"

#include "00. Engine/Manager/ResourceManager/Resource/Animation.h"
#include "02. GameContents/Item/Button.h"
#include "00. Engine/Component/Collider/Collider.h"
#include "00. Engine/Component/Collider/RectCollider.h"
#include "00. Engine/Component/MeshRenderer/MeshRenderer.h"
#include "02. GameContents/Item/Wall.h"

Button::Button(Vec3 buttonPosition)
{
	GetTransform()->SetPosition(buttonPosition);

	MeshRenderer* meshRenderer = new MeshRenderer();
	AddComponent(meshRenderer);
	meshRenderer->SetRenderingOrder(2);
	auto mesh = GGameManager->GetResourceManager()->Get<Mesh>(L"Bottom0.5x0.5");
	meshRenderer->SetMesh(mesh);
	auto material = GGameManager->GetResourceManager()->Get<Material>(L"Default");
	meshRenderer->SetMaterial(material);

	Animator* animator = new Animator();
	AddComponent(animator);
	auto animation = GAME->GetResourceManager()->Get<Animation>(L"buttonUp");
	animator->AddAnimation(0, animation);
	animation = GAME->GetResourceManager()->Get<Animation>(L"buttonDown");
	animator->AddAnimation(1, animation);
	animator->SetPresentAnimation(0);

	Collider* collider = new RectCollider("Button");
	vector<Vec3> vertices(5);
	vertices[0] = Vec3(-0.2f, 0.4f, 0.f);
	vertices[1] = Vec3(0.2f, 0.4f, 0.f);
	vertices[2] = Vec3(0.2f, 0.f, 0.f);
	vertices[3] = Vec3(-0.2f, 0.f, 0.f);
	vertices[4] = Vec3(-0.2f, 0.4f, 0.f);

	collider->SetVertices(vertices);
	AddComponent(collider);
}

Button::Button(Vec3 buttonPosition, float _restoreTime)
{
	GetTransform()->SetPosition(buttonPosition);
	restore = true;
	restoreTime = _restoreTime;
}

Button::~Button()
{

}

void Button::Init()
{
	__super::Init();
	pushed = false;
	GetCollider()->Activate();
	GetAnimator()->SetPresentAnimation(0);
}

void Button::UpdateObject()
{
	if (restore)
	{
		if (restoreTimeCalc < restoreTime)
		{
			restoreTimeCalc += Time->GetDeltaTime();
		}
		else
		{
			pushed = false;
			GetCollider()->Activate();
			GetAnimator()->SetPresentAnimation(0);
			restoreTimeCalc = 0.f;
		}
	}
}

void Button::PushButton()
{
	pushed = true;
	GetCollider()->Inactivate();
	GetAnimator()->SetPresentAnimation(1);

	for (Wall* wall : wallList)
	{
		wall->SetDown(false);
	}
}

Wall* Button::AddWall(Vec3 wallPosition)
{
	Wall* wall = new Wall(wallPosition);
	wallList.push_back(wall);
	return wall;
}