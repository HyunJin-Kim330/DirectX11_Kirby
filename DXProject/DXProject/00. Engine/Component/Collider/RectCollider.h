#pragma once
#include "Collider.h"

class RectCollider : public Collider
{
	using Super = Collider;

public:
	RectCollider(string _tag);
	~RectCollider();

protected:
	virtual pair<bool, CollisionDirection> CheckCollision(Collider* collider) override;

private:
	Vec2 position;
	Vec2 size;
};

