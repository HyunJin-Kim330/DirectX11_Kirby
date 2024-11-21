#pragma once
#include "Component.h"
#include "VertexBuffer.h"

class CollisionBehavior;

/*
	구현되어 있는 충돌감지

	원 - 선
	원 - 사각형
	사각형 - 사각형
*/

enum class Shape  // 충돌체 모양
{
	Rectangle,
	Line,
	Dot,
};

enum class CollisionDirection  // 충돌 방향
{
	Right,
	Left,
	Top,
	Bottom
};

class Collider : public Component
{
	using Super = Component;

public:
	Collider(Shape _shape);
	virtual ~Collider();

public:
	virtual void Update() override;

	vector<Vec3> GetOriginalVertices();
	vector<Vec3> GetVertices();
	VertexBuffer* GetVertexBuffer() const { return vertexBuffer; }
	Shape GetShape() const { return shape; }
	string GetTag() const { return tag; }
	bool GetActive() const { return active; }

	void SetTag(string _tag) { tag = _tag; }
	virtual void SetVertices(vector<Vec3>& _vertices);
	void SetCollisionBehavior(CollisionBehavior* behavior) { collisionBehavior = behavior; }

	void Inactivate() { active = false; }			// 콜라이더 비활성화
	void Activate() { active = true; }			// 콜라이더 활성화
	void AddCheckingTag(string tag) { checkingTag.insert(tag); }		// 체크할 태그 추가

protected:
	bool active = true;
	ComPtr<ID3D11Device> device;
	Shape shape = Shape::Rectangle;						// 콜라이더 모양
	string tag;													// 콜라이더의 태그
	vector<Vec3> vertices;									// 콜라이더의 정점
	VertexBuffer* vertexBuffer;								// 다렉에서 사용해야 할 버텍스버퍼
	set<string> checkingTag;								// 이 콜라이더가 체크해야 할 태그 목록
	CollisionBehavior* collisionBehavior = nullptr;		// 충돌 시 어떤행위를 할 지 (전략)
	
protected:
	vector<Collider*> BringCheckingObject();
	virtual pair<bool, CollisionDirection>  CheckCollision(Collider* collider) abstract;

};

