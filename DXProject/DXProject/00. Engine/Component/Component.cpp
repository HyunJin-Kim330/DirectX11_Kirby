#include "pch.h"
#include "00. Engine/Component/Component.h"
#include "00. Engine/Component/Transform/Transform.h"

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