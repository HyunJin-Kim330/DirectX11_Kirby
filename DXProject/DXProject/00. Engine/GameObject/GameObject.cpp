#include "pch.h"
#include "00. Engine/GameObject/GameObject.h"
#include "00. Engine/Component/Camera/Camera.h"
#include "00. Engine/Component/MeshRenderer/MeshRenderer.h"
#include "00. Engine/Component/Animator/Animator.h"
#include "00. Engine/Component/Collider/Collider.h"
#include "00. Engine/Manager/SceneManager/SceneManager.h"

GameObject::GameObject()
{
	Transform* transform = new Transform();
	AddComponent(transform);
}

GameObject::~GameObject()
{
	for (auto& com : components)
	{
		delete com;
	}
}

void GameObject::Init()
{
	GetTransform()->SetPosition(initPosition);
}

void GameObject::Update()
{
	UpdateComponent();
	UpdateObject();
}

void GameObject::UpdateComponent()
{
	for (Component* component : components)
	{
		if (component)
			component->Update();
	}
}

void GameObject::AddComponent(Component* component)
{
	component->SetGameObject(this);

	uint8 index = static_cast<uint8>(component->GetType());
	components[index] = component;
}

Component* GameObject::GetFixedComponent(ComponentType type)
{
	uint8 index = static_cast<uint8>(type);
	return components[index];
}

Transform* GameObject::GetTransform()
{
	Component* component = GetFixedComponent(ComponentType::Transform);
	return dynamic_cast<Transform*>(component);
}

Camera* GameObject::GetCamera()
{
	Component* component = GetFixedComponent(ComponentType::Camera);
	return dynamic_cast<Camera*>(component);
}

MeshRenderer* GameObject::GetMeshRenderer()
{
	Component* component = GetFixedComponent(ComponentType::MeshRenderer);
	return dynamic_cast<MeshRenderer*>(component);
}

Animator* GameObject::GetAnimator()
{
	Component* component = GetFixedComponent(ComponentType::Animator);
	return dynamic_cast<Animator*>(component);
}

Collider* GameObject::GetCollider()
{
	Component* component = GetFixedComponent(ComponentType::Collider);
	return dynamic_cast<Collider*>(component);
}

void GameObject::AddColliderWithVector(vector<Collider*>& colliders, string tag)
{
	Component* component = GetFixedComponent(ComponentType::Collider);

	if (component == nullptr) return;

	Collider* collider = dynamic_cast<Collider*>(component);
	if (collider->GetTag() == tag)
	{
		colliders.push_back(collider);
	}
}

void GameObject::AddGameObjectToScene(GameObject* newGameObject)
{
	GAME->GetSceneManager()->GetActiveScene()->AddGameObject(newGameObject);
}

GameObject* GameObject::GetParent()
{
	return parent;
}

//vector<GameObject*> GameObject::GetChildren()
//{
//	return children;
//}

list<GameObject*>& GameObject::GetChildren()
{
	return children;
}

void GameObject::SetParent(GameObject* gameObject)
{
	parent = gameObject;
}

void GameObject::AddChild(GameObject* gameObject)
{
	children.push_back(gameObject);
}

void GameObject::DeleteChild(GameObject* gameObject)
{
	children.remove(gameObject);
}

Vec3 GameObject::GetMidstPosition()
{
	vector<VertexTextureData>& vertices = GetMeshRenderer()->GetMesh()->GetGeometry()->GetVertices();
	vector<Vec3> vertexList;
	Matrix matWorld = GetTransform()->GetWorldMatrix();

	for (const VertexTextureData& vertex : vertices)
	{
		vertexList.push_back(Vec3::Transform(vertex.position, matWorld));
	}

	float x = vertexList[0].x + ((vertexList[2].x - vertexList[0].x) / 2.f);
	float y = vertexList[0].y - ((vertexList[0].y - vertexList[1].y) / 2.f);

	return Vec3{ x, y, 0.f };
}