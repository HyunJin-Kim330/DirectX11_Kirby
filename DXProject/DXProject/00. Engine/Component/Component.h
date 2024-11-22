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
	Transform* GetTransform();		// 컴포넌트 주인의 transform
	GameObject* GetGameObject();	// 컴포넌트 주인 리턴

private:
	friend class GameObject;
	void SetGameObject(GameObject* _gameObject) { gameObject = _gameObject; }

private:
	ComponentType type;

protected:
	GameObject* gameObject;		// 컴포넌트 주인
};

