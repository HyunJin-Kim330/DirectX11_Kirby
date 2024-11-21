#pragma once

class GameObject;
class Transform;

enum class ComponentType : uint8
{
	Transform,
	Camera,
	MeshRenderer,
	Animator,
	Collider,
	End
};

enum
{
	COMPONENT_TYPE_COUNT = (ComponentType::End)
};

class Component
{
public:
	Component(ComponentType _type);
	virtual ~Component();

	virtual void Update() abstract;

public:
	ComponentType GetType() { return type; }
	Transform* GetTransform();		// ������Ʈ ������ transform
	GameObject* GetGameObject();	// ������Ʈ ���� ����

private:
	friend class GameObject;
	void SetGameObject(GameObject* _gameObject) { gameObject = _gameObject; }

private:
	ComponentType type;

protected:
	GameObject* gameObject;		// ������Ʈ ����
};

