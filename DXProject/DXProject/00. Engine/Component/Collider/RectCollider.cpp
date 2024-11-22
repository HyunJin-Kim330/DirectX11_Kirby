#include "pch.h"
#include "RectCollider.h"
#include <iostream>

RectCollider::RectCollider(string _tag) : Super(Shape::Rectangle)
{
	tag = _tag;
}

RectCollider::~RectCollider()
{

}

pair<bool, CollisionDirection> RectCollider::CheckCollision(Collider* collider)  // AABB
{
	if (collider->GetShape() == Shape::Rectangle)
	{
		vector<Vec3> v1 = collider->GetVertices();
		vector<Vec3> v2 = this->GetVertices();

		float r1HalfWidth = abs((v1[1].x - v1[0].x) / 2.f);
		float r2HalfWidth = abs((v2[1].x - v2[0].x) / 2.f);
		float r1HalfHeight = abs((v1[3].y - v1[0].y) / 2.f);
		float r2HalfHeight = abs((v2[3].y - v2[0].y) / 2.f);

		r1HalfWidth *= 10;
		r1HalfWidth = roundf(r1HalfWidth);
		r1HalfWidth /= 10;

		r2HalfWidth *= 10;
		r2HalfWidth = roundf(r2HalfWidth);
		r2HalfWidth /= 10;

		r1HalfHeight *= 10;
		r1HalfHeight = roundf(r1HalfHeight);
		r1HalfHeight /= 10;

		r2HalfHeight *= 10;
		r2HalfHeight = roundf(r2HalfHeight);
		r2HalfHeight /= 10;

		Vec2 r1Position = Vec2{ v1[0].x + r1HalfWidth, v1[0].y - r1HalfHeight };
		Vec2 r2Position = Vec2{ v2[0].x + r2HalfWidth, v2[0].y - r2HalfHeight };

		//float distance = sqrtf(pow(r1Position.x - r2Position.x, 2) + pow(r1Position.y - r2Position.y, 2));
		float xDistance = fabsf(r1Position.x - r2Position.x);
		float yDistance = fabsf(r1Position.y - r2Position.y);


		Vec2 distance = r1Position - r2Position;
		float xOverlap = (r1HalfWidth + r2HalfWidth) - abs(distance.x);
		float yOverlap = (r1HalfHeight + r2HalfHeight) - abs(distance.y);

		CollisionDirection direction = CollisionDirection::Right;
		if (yOverlap > xOverlap)
		{
			direction = (distance.x > 0) ? CollisionDirection::Right : CollisionDirection::Left;
			if(v2[3].y + 0.01 > v1[1].y) return { false, direction };
		}
		else
		{
			direction = (distance.y > 0) ? CollisionDirection::Top : CollisionDirection::Bottom;
		}

		if (r1HalfWidth + r2HalfWidth > xDistance && r1HalfHeight + r2HalfHeight > yDistance)
			return { true, direction };
		else
			return { false, direction };
	}
		

}