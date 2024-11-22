#pragma once
#include "Collider.h"

class LineCollider : public Collider
{
	using Super = Collider;

public:
	LineCollider(string _tag);
	~LineCollider();

	bool GetTiltLeft() const { return tiltLeft; }
	Vec3 GetLeftDirectionVec() const { return leftDirectionVec; }
	Vec3 GetRightDirectionVec() const { return rightDirectionVec; }

	virtual void SetVertices(vector<Vec3>& _vertices);

protected:
	virtual pair<bool, CollisionDirection> CheckCollision(Collider* collider) override;

private:
	bool tiltLeft = false;
	Vec3 leftDirectionVec = { 0.f, 0.f, 0.f };
	Vec3 rightDirectionVec = { 0.f, 0.f, 0.f };

private:
	bool CollideLineToLine(Vec3 otherStartPoint, Vec3 otherEndPoint);
	float CCW(float x1, float y1, float x2, float y2, float x3, float y3);

};

