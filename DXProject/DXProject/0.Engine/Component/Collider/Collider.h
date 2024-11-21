#pragma once
#include "Component.h"
#include "VertexBuffer.h"

class CollisionBehavior;

/*
	�����Ǿ� �ִ� �浹����

	�� - ��
	�� - �簢��
	�簢�� - �簢��
*/

enum class Shape  // �浹ü ���
{
	Rectangle,
	Line,
	Dot,
};

enum class CollisionDirection  // �浹 ����
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

	void Inactivate() { active = false; }			// �ݶ��̴� ��Ȱ��ȭ
	void Activate() { active = true; }			// �ݶ��̴� Ȱ��ȭ
	void AddCheckingTag(string tag) { checkingTag.insert(tag); }		// üũ�� �±� �߰�

protected:
	bool active = true;
	ComPtr<ID3D11Device> device;
	Shape shape = Shape::Rectangle;						// �ݶ��̴� ���
	string tag;													// �ݶ��̴��� �±�
	vector<Vec3> vertices;									// �ݶ��̴��� ����
	VertexBuffer* vertexBuffer;								// �ٷ����� ����ؾ� �� ���ؽ�����
	set<string> checkingTag;								// �� �ݶ��̴��� üũ�ؾ� �� �±� ���
	CollisionBehavior* collisionBehavior = nullptr;		// �浹 �� ������� �� �� (����)
	
protected:
	vector<Collider*> BringCheckingObject();
	virtual pair<bool, CollisionDirection>  CheckCollision(Collider* collider) abstract;

};

