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
	void Update();  // UpdateComponent�� UpdateObject�� ȣ��
	
	void AddComponent(Component* component);
	Component* GetFixedComponent(ComponentType type);
	Transform* GetTransform();
	Camera* GetCamera();
	MeshRenderer* GetMeshRenderer();
	Animator* GetAnimator();
	Collider* GetCollider();
	void AddColliderWithVector(vector<Collider*>& colliders, string tag);		// �����ִ� collider�� tag�� �Ű������� ���Ͽ� ���ٸ� vector�� �߰�

	GameObject* GetParent();
	void SetParent(GameObject* gameObject);
	list<GameObject*>& GetChildren();

	void AddChild(GameObject* gameObject);
	void DeleteChild(GameObject* gameObject);
	void AddGameObjectToScene(GameObject* newGameObject);
	void SetInitPosition(Vec3 position) { initPosition = position; }

	Vec3 GetMidstPosition();		// ���� �޽� vertex�� �� ��� ��ǥ

private:
	void UpdateComponent();

protected:
	array<Component*, COMPONENT_TYPE_COUNT> components = { nullptr, };
	GameObject* parent = nullptr;
	list<GameObject*> children;
	Vec3 initPosition = Vec3{ 0.f, 0.f, 0.f };

protected:
	virtual void UpdateObject() abstract;		// GameObject�� ���� ������Ʈ

};

