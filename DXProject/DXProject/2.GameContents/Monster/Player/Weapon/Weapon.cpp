#include "pch.h"
#include "Weapon.h"
#include "Collider.h"
#include "MeshRenderer.h"
#include "Scene.h"
#include "SceneManager.h"

void Weapon::UpdateObject()
{

}

void Weapon::StopAttack(bool deleteObject)
{
	if (GetMeshRenderer() != nullptr)
	{
		GetMeshRenderer()->InactiveMeshRenderer();  // 공격이 멈추면 렌더링하지 않도록
	}

	if (GetCollider() != nullptr)
	{
		GetCollider()->Inactivate();
	}

	/*if (dependeOnSummonerPosition)
	{
		GetParent()->DeleteChild(this);
		SetParent(nullptr);
	}*/

	attackTime = 0.f;

	GAME->GetSceneManager()->GetActiveScene()->RemoveGameObject(this, deleteObject);

}