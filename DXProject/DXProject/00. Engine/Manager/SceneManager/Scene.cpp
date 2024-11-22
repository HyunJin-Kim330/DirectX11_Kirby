#include "pch.h"
#include "00. Engine/Component/Collider/Collider.h"
#include "00. Engine/Manager/SceneManager/Scene.h"
#include "00. Engine/Component/MeshRenderer/MeshRenderer.h"


Scene::Scene(pair<float, float> cameraX, pair<float, float> cameraY, string _name) : cameraX(cameraX), cameraY(cameraY)
{
	name = _name;
	//gameObjectList.resize(10);
}

Scene::~Scene()
{

}

void Scene::Init()
{

}

void Scene::Update()
{
	for (GameObject* gameObject : gameObjectList) // list로 바꿔야할까?
	{
		gameObject->Update();
	}

	for (pair<GameObject*, bool> gameObject : removeObjectList)
	{
		set<GameObject*>::iterator iter = gameObjectList.find(gameObject.first);

		if (iter != gameObjectList.end())
		{
			gameObjectList.erase(iter);
		}

		if (gameObject.second)
		{
			delete gameObject.first;
		}
	}

	removeObjectList.clear();
}

void Scene::AddGameObject(GameObject* gameObject)
{
	gameObjectList.insert(gameObject);
}

void Scene::RemoveGameObject(GameObject* gameObject, bool deleteObject)
{
	removeObjectList.insert({ gameObject, deleteObject });
}

vector<GameObject*> Scene::GetRenderableObject()
{
	vector<GameObject*> renderableObject;

	for (GameObject* object : gameObjectList)
	{
		MeshRenderer* meshRenderer = object->GetMeshRenderer();
		if (meshRenderer != nullptr && meshRenderer->GetActive())
		{
			renderableObject.push_back(object);
		}
	}

	sort(renderableObject.begin(), renderableObject.end(), RendererOrder());
	return renderableObject;
}

vector<GameObject*> Scene::GetRenderableCollider()
{
	vector<GameObject*> renderableObject;
	for (GameObject* object : gameObjectList)
	{
		Collider* collider = object->GetCollider();
		if (object->GetCollider() != nullptr && collider->GetActive())
		{
			renderableObject.push_back(object);
		}
	}
	return renderableObject;
}

vector<Collider*> Scene::GetColliders(string tag)
{
	vector<Collider*> colliders;
	
	for (GameObject* gameObject : gameObjectList)
	{
		gameObject->AddColliderWithVector(colliders, tag);
	}
	
	return colliders;
}