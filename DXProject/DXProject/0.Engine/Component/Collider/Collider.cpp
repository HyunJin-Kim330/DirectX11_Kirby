#include "pch.h"
#include "Camera.h"
#include "Collider.h"
#include "CollisionBehavior.h"
#include "RenderManager.h"
#include "SceneManager.h"

Collider::Collider(Shape _shape) : Super(ComponentType::Collider), shape(_shape)
{
	device = GAME->GetRenderManager()->GetDevice();
}

Collider::~Collider()
{

}

void Collider::Update()
{
	if (!active) return;

	set<pair<string, CollisionDirection>> collidedTag;


	for (Collider* collider : BringCheckingObject())
	{
		if (!collider->GetActive()) continue;
		pair<bool, CollisionDirection> result = CheckCollision(collider);
		bool isCollide = result.first;
		CollisionDirection collisionDirection = result.second;

		if (isCollide)
		{
			collisionBehavior->Behavior(collider, this, collisionDirection);
			collidedTag.insert({collider->GetTag(), collisionDirection});
		}
	}

	if (collisionBehavior != nullptr)
	{
		collisionBehavior->NotAtAllCollideBehavior(collidedTag, this);
	}
}

vector<Vec3> Collider::GetOriginalVertices()
{
	return vertices;
}

vector<Vec3> Collider::GetVertices()
{
	vector<Vec3> vertexList;
	Matrix matWorld = GetTransform()->GetWorldMatrix();

	for (Vec3 vertex : vertices)
	{
		vertexList.push_back(Vec3::Transform(vertex, matWorld));
	}

	return vertexList;
}

vector<Collider*> Collider::BringCheckingObject()
{
	vector<Collider*> checkingObjects;
	set<string>::iterator iter = checkingTag.begin();
	for (; iter != checkingTag.end(); ++iter)
	{
		vector<Collider*> temp = GAME->GetSceneManager()->GetActiveScene()->GetColliders(*iter);
		if(temp.size())
			checkingObjects.insert(checkingObjects.end(), temp.begin(), temp.end());
	}

	return checkingObjects;
}

bool RayVsRect(Collider* c1, Collider* c2)
{
	vector<Vec3> v1 = c1->GetVertices();
	vector<Vec3> v2 = c2->GetVertices();

	Vec2 rayOrigin = Vec2{ (v1[0] + v1[2]) / 2 };

	return true;
}

void Collider::SetVertices(vector<Vec3>& _vertices)
{
	vertices = _vertices;
	vertexBuffer = new VertexBuffer(device);
	vertexBuffer->Create(vertices);
}