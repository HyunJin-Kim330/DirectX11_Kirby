#include "pch.h"
#include "CollisionBehavior.h"
#include "00. Engine/Component/Collider/Collider.h"

Direction CollisionBehavior::CollideLeftOrRight(Collider* other, Collider* standard, CollisionDirection colDirection)
{
	if (colDirection == CollisionDirection::Left)
	{
		return Direction::LEFT;
	}
	else if (colDirection == CollisionDirection::Right)
	{
		return Direction::RIGHT;
	}
	else
	{
		float standardPosition = standard->GetGameObject()->GetTransform()->GetPosition().x;
		float otherPosition = other->GetGameObject()->GetTransform()->GetPosition().x;

		return (standardPosition >= otherPosition ? Direction::LEFT : Direction::RIGHT);
	}
}