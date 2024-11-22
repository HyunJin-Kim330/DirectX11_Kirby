#pragma once
#include "00. Engine/Component/Component.h"
class GameObject;

class Transform : public Component
{
	using Super = Component;
public:
	Transform();
	~Transform();

public:

	virtual void Update() override;

	Matrix GetWorldMatrix() { return worldMatrix; }

public:
	void SetLocalScale(const Vec3& scale);
	void SetLocalRotation(const Vec3& rotation);
	void SetLocalPosition(const Vec3& position);
	void SetLocalSTR(const Vec3& scale, const Vec3& rotation, const Vec3& position);

	/*
		setScale, setRotation, setPosition메소드는
		월드 기준으로 어떤 값을 설정했을 때 상하관계를 따져
		다시 local에 관한 값을 조정하는 메소드들 입니다
	*/
	Vec3 GetScale() { return worldScale; }
	void SetScale(const Vec3& scale);
	Vec3 GetRotation() { return worldRotation; }
	void SetRotation(const Vec3& rotation);
	Vec3 GetPosition() { return worldPosition; }
	void SetPosition(const Vec3& position);

	Vec3 GetRight() { return worldMatrix.Right(); }
	Vec3 GetUp() { return worldMatrix.Up(); }
	Vec3 GetLook() { return worldMatrix.Backward(); }

private:
	void UpdateMatrix();

private:
	Vec3 localScale = { 1.f, 1.f, 1.f };
	Vec3 localRotation = { 0.f, 0.f, 0.f };
	Vec3 localPosition = { 0.f, 0.f, 0.f };

	Vec3 worldScale;
	Vec3 worldRotation;
	Vec3 worldPosition;

	Matrix localMatrix = Matrix::Identity;
	Matrix worldMatrix = Matrix::Identity;

};

