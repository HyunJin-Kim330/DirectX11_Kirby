#pragma once
#include "Collider.h"

class DotCollider : public Collider
{
	using Super = Collider;

public:
	DotCollider(string _tag, float _radius);
	~DotCollider();

protected:
	virtual pair<bool, CollisionDirection> CheckCollision(Collider* collider) override;

private:
	float radius = 0.f;		// 원 충돌체 반지름

};

