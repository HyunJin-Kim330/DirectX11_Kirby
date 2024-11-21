#pragma once
#include "Component.h"

class Animator;
class Camera;
class Collider;
class MeshRenderer;
class Scene;

class GameObject
{
public:
	GameObject();
	virtual ~GameObject();

public:
	virtual void Init();
	void Update();  // UpdateComponent와 UpdateObject를 호출
	
	void AddComponent(Component* component);
	Component* GetFixedComponent(ComponentType type);
	Transform* GetTransform();
	Camera* GetCamera();
	MeshRenderer* GetMeshRenderer();
	Animator* GetAnimator();
	Collider* GetCollider();
	void AddColliderWithVector(vector<Collider*>& colliders, string tag);		// 갖고있는 collider의 tag와 매개변수를 비교하여 같다면 vector에 추가

	GameObject* GetParent();
	void SetParent(GameObject* gameObject);
	list<GameObject*>& GetChildren();

	void AddChild(GameObject* gameObject);
	void DeleteChild(GameObject* gameObject);
	void AddGameObjectToScene(GameObject* newGameObject);
	void SetInitPosition(Vec3 position) { initPosition = position; }

	Vec3 GetMidstPosition();		// 계산된 메쉬 vertex의 정 가운데 좌표

private:
	void UpdateComponent();

protected:
	array<Component*, COMPONENT_TYPE_COUNT> components = { nullptr, };
	GameObject* parent = nullptr;
	list<GameObject*> children;
	Vec3 initPosition = Vec3{ 0.f, 0.f, 0.f };

protected:
	virtual void UpdateObject() abstract;		// GameObject별 개별 업데이트

};

