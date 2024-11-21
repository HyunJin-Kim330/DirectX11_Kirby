#include "pch.h"
#include "Animation.h"
#include "Animator.h"
#include "EffectObject.h"
#include "MeshRenderer.h"
#include "ResourceManager.h"
#include "Scene.h"
#include "SceneManager.h"

EffectObject::EffectObject(wstring _animName, float _lifeTime, wstring _mesh, wstring _material)
{
	lifeTime = _lifeTime;

	// add MeshRenderer
	MeshRenderer* meshRenderer = new MeshRenderer();
	AddComponent(meshRenderer);
	auto mesh = GGameManager->GetResourceManager()->Get<Mesh>(_mesh);
	meshRenderer->SetMesh(mesh);
	auto material = GGameManager->GetResourceManager()->Get<Material>(_material);
	meshRenderer->SetMaterial(material);
	meshRenderer->SetRenderingOrder(4);

	// add Animator
	Animator* animator = new Animator();
	AddComponent(animator);

	Animation* animation = GAME->GetResourceManager()->Get<Animation>(_animName);
	animator->AddAnimation(0, animation);
	animator->SetPresentAnimation(0);
}

EffectObject::~EffectObject()
{

}

void EffectObject::UpdateObject()
{
	calcLifeTime += Time->GetDeltaTime();

	if (calcLifeTime >= lifeTime)
	{
		calcLifeTime = 0.f;
		GetAnimator()->InitFrameNum();
		GAME->GetSceneManager()->GetActiveScene()->RemoveGameObject(this, false);  // 애니메이션 시간이 다 끝나면 씬에서 제외되도록(객체는 delete되지않음)
	}
}