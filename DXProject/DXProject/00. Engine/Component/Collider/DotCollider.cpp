#include "pch.h"
#include "DotCollider.h"
#include <string>

DotCollider::DotCollider(string _tag, float _radius) : Super(Shape::Dot)
{
	tag = _tag;
	radius = _radius;
}

DotCollider::~DotCollider()
{

}

//pair<bool, CollisionDirection> DotCollider::CheckCollision(Collider* collider)
//{
//	// 내가 점, 상대방이 선
//	if (collider->GetShape() == Shape::Line)
//	{
//		vector<Vec3> line = collider->GetVertices();
//		Vec3 dot = this->GetVertices()[0];
//
//		Vec3 first = line[0];
//		Vec3 second = line[1];
//		
//		float lineLength;
//		float firstWithDotLength;
//		float secondWithDotLength;
//
//		{
//			float a = second.x - first.x;
//			float b = second.y - first.y;
//			lineLength = sqrtf((a * a) + (b * b));
//		}
//		{
//			float a = first.x - dot.x;
//			float b = first.y - dot.y;
//			firstWithDotLength = sqrtf((a * a) + (b * b));
//		}
//		{
//			float a = second.x - dot.x;
//			float b = second.y - dot.y;
//			secondWithDotLength = sqrtf((a * a) + (b * b));
//		}
//
//		firstWithDotLength -= 0.001f;
//
//		/*
//		secondWithDotLength *= 10;
//		secondWithDotLength = roundf(secondWithDotLength);
//		secondWithDotLength /= 10;*/
//	}
//
//	return { false, CollisionDirection::Left };
//}


pair<bool, CollisionDirection> DotCollider::CheckCollision(Collider* collider)
{
	// 내가 원, 상대방이 선
	if (collider->GetShape() == Shape::Line)
	{
		vector<Vec3> line = collider->GetVertices();
		Vec3 focus = this->GetVertices()[0];

		if(focus.x < line[0].x || focus.x > line[1].x) return { false, CollisionDirection::Left };
		
		Vec2 lineVec = Vec2{ line[1].x - line[0].x, line[1].y - line[0].y };
		Vec2 lineToCircleVec = Vec2{ focus.x - line[0].x, focus.y - line[0].y };

		Vec2 resultOfMul = lineVec * lineToCircleVec;
		float resultOfDotProduct = resultOfMul.x + resultOfMul.y;

		if(resultOfDotProduct <= 0) return { false, CollisionDirection::Left };

		float length = 0.f;
		{
			float a = line[1].x - line[0].x;
			float b = line[1].y - line[0].y;

			float lengthOfLine = sqrtf(a * a + b * b);
			length = resultOfDotProduct / lengthOfLine;
		}

		float lengthOfLineStartWithFocus = 0.f;
		{
			float a = focus.x - line[0].x;
			float b = focus.y - line[0].y;

			lengthOfLineStartWithFocus = sqrtf(a * a + b * b);
		}

		float lengthOfLineToFocus = sqrtf(lengthOfLineStartWithFocus * lengthOfLineStartWithFocus - length * length);

		if (radius >= lengthOfLineToFocus)
		{
			return { true, CollisionDirection::Left };
		}
	}
	else if (collider->GetShape() == Shape::Rectangle)
	{
		vector<Vec3> rectangle = collider->GetVertices();
		Vec3 focus = this->GetVertices()[0];

		if (rectangle[0].x < focus.x && focus.x < rectangle[1].x)
		{
			if (rectangle[0].y < focus.y && focus.y < rectangle[0].y + radius)
			{
				return { true, CollisionDirection::Bottom };
			}
		}

		if (rectangle[1].x < focus.x && focus.x < rectangle[1].x + radius)
		{
			if (rectangle[1].y > focus.y && focus.y > rectangle[2].y)
			{
				return { true, CollisionDirection::Left };
			}
		}

		if (rectangle[3].x < focus.x && focus.x < rectangle[2].x)
		{
			if (rectangle[3].y > focus.y && focus.y > rectangle[3].y - radius)
			{
				return { true, CollisionDirection::Top };
			}
		}

		if (rectangle[0].x > focus.x && focus.x > rectangle[0].x - radius)
		{
			if (rectangle[0].y > focus.y && focus.y > rectangle[3].y)
			{
				return { true, CollisionDirection::Right };
			}
		}

		for (int i = 0; i < 4; i++)
		{
			Vec3 minusVec = rectangle[i] - focus;

			if (sqrtf(minusVec.x * minusVec.x + minusVec.y * minusVec.y) <= radius)
			{
				if (i == 0 || i == 3)
				{
					return { true, CollisionDirection::Right };
				}
				else
				{
					return { true, CollisionDirection::Left };
				}
			}
		}
	}

	return { false, CollisionDirection::Left };
}