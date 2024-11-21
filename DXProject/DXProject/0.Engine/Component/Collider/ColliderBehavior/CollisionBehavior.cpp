#include "pch.h"
#include "CollisionBehavior.h"
#include "Collider.h"

Direction CollisionBehavior::CollideLeftOrRight(Collider* citizen, Collider* standard, CollisionDirection colDirection)
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
		float citizenPosition = citizen->GetGameObject()->GetTransform()->GetPosition().x;

		return (standardPosition >= citizenPosition ? Direction::LEFT : Direction::RIGHT);
	}
}