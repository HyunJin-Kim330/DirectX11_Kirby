#include "pch.h"
#include "LineCollider.h"

LineCollider::LineCollider(string _tag) : Super(Shape::Line)
{
	tag = _tag;
}

LineCollider::~LineCollider()
{

}

void LineCollider::SetVertices(vector<Vec3>& _vertices)
{
	vertices = _vertices;
	vertexBuffer = new VertexBuffer(device);
	vertexBuffer->Create(vertices);

	tiltLeft = vertices[0].y < vertices[1].y ? true : false;
	leftDirectionVec = vertices[0] - vertices[1];
	rightDirectionVec = vertices[1] - vertices[0];
}

pair<bool, CollisionDirection> LineCollider::CheckCollision(Collider* other)
{
	if (other->GetShape() == Shape::Line)
	{
		bool result = CollideLineToLine(other->GetVertices()[0], other->GetVertices()[1]);

		return { result, CollisionDirection::Left };
	}
	else if (other->GetShape() == Shape::Rectangle)
	{
		vector<Vec3> otherVertices = other->GetVertices();

		for (int i = 0; i < 4; i++)
		{
			bool result = CollideLineToLine(otherVertices[i], otherVertices[i + 1]);
			if (result)
			{
				return { true, CollisionDirection::Left };
			}
		}
		return { false, CollisionDirection::Left };
	}
	else
	{
		return { false, CollisionDirection::Left };
	}
}

bool LineCollider::CollideLineToLine(Vec3 otherStartPoint, Vec3 otherEndPoint)
{
	vector<Vec3> myLine = GetVertices();
	Vec3 myStartPoint = myLine[0];
	Vec3 myEndPoint = myLine[1];

	float result = CCW(otherStartPoint.x, otherStartPoint.y, otherEndPoint.x, otherEndPoint.y, myStartPoint.x, myStartPoint.y);
	if (result == 0.f) return false;

	float result2 = CCW(otherStartPoint.x, otherStartPoint.y, otherEndPoint.x, otherEndPoint.y, myEndPoint.x, myEndPoint.y);
	if (result2 == 0.f) return false;

	if (result * result2 < 0.f)
	{
		result = CCW(myStartPoint.x, myStartPoint.y, myEndPoint.x, myEndPoint.y, otherStartPoint.x, otherStartPoint.y);
		if (result == 0.f) return false;

		result2 = CCW(myStartPoint.x, myStartPoint.y, myEndPoint.x, myEndPoint.y, otherEndPoint.x, otherEndPoint.y);
		if (result2 == 0) return false;

		if (result * result2 < 0)
		{
			return true;
		}
	}

	return false;
}

float LineCollider::CCW(float x1, float y1, float x2, float y2, float x3, float y3)
{
	return (x2 - x1) * (y3 - y1) - (x3 - x1) * (y2 - y1);
}