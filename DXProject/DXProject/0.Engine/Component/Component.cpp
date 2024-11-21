#include "pch.h"
#include "Component.h"
#include "Transform.h"

Component::Component(ComponentType _type) : type(_type)
{

}

Component::~Component()
{

}

GameObject* Component::GetGameObject()
{
	return gameObject;
}

Transform* Component::GetTransform()
{
	return gameObject->GetTransform();
}