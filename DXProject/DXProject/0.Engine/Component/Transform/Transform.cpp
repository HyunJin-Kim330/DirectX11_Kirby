#include "pch.h"
#include "Transform.h"

Transform::Transform() : Super(ComponentType::Transform)
{

}

Transform::~Transform()
{

}

void Transform::Update()
{

}

Vec3 ToEulerAngles(Quaternion q)
{
	Vec3 angles;

	// roll (x-axis rotation)
	double sinr_cosp = 2 * (q.w * q.x + q.y * q.z);
	double cosr_cosp = 1 - 2 * (q.x * q.x + q.y * q.y);
	angles.x = std::atan2(sinr_cosp, cosr_cosp);

	// pitch (y-axis rotation)
	double sinp = std::sqrt(1 + 2 * (q.w * q.y - q.x * q.z));
	double cosp = std::sqrt(1 - 2 * (q.w * q.y - q.x * q.z));
	angles.y = 2 * std::atan2(sinp, cosp) - 3.14159f / 2;

	// yaw (z-axis rotation)
	double siny_cosp = 2 * (q.w * q.z + q.x * q.y);
	double cosy_cosp = 1 - 2 * (q.y * q.y + q.z * q.z);
	angles.z = std::atan2(siny_cosp, cosy_cosp);

	return angles;
}

void Transform::UpdateMatrix()
{
	Matrix scaleMat = Matrix::CreateScale(localScale);
	Matrix rotateMat = Matrix::CreateRotationX(localRotation.x);
	rotateMat *= Matrix::CreateRotationY(localRotation.y);
	rotateMat *= Matrix::CreateRotationZ(localRotation.z);
	Matrix transMat = Matrix::CreateTranslation(localPosition);

	localMatrix = scaleMat * rotateMat * transMat;

	if (gameObject->GetParent() == nullptr)  // 부모의 유무에 따라 월드 변환행렬이 달라짐
	{
		worldMatrix = localMatrix;
	}
	else
	{
		Matrix parentWorldMatrix = gameObject->GetParent()->GetTransform()->GetWorldMatrix();
		worldMatrix = localMatrix * parentWorldMatrix;
	}

	Quaternion quat;
	worldMatrix.Decompose(worldScale, quat, worldPosition);
	worldRotation = ToEulerAngles(quat);

	for (GameObject* child : gameObject->GetChildren())
	{
		child->GetTransform()->UpdateMatrix(); 
	}
}

/*
	부모 local속성값을 바꾼다고 해도같이 움직이거나 같이 커지거나 같이 회전하기 때문에
	자식 local값은 그대로이며, world에 관련된 값만 다시 설정해주면 된다
*/

void Transform::SetLocalSTR(const Vec3 & scale, const Vec3 & rotation, const Vec3 & position)
{
	localScale = scale;
	localRotation = rotation;
	localPosition = position;
	UpdateMatrix();
}

void Transform::SetLocalScale(const Vec3& scale)
{ 
	localScale = scale; 
	UpdateMatrix(); 
}

void Transform::SetLocalRotation(const Vec3& rotation)
{ 
	localRotation = rotation; 
	UpdateMatrix(); 
}

void Transform::SetLocalPosition(const Vec3& position)
{ 
	localPosition = position; 
	UpdateMatrix(); 
}

void Transform::SetScale(const Vec3& worldScale)
{
	if (gameObject->GetParent() == nullptr)
	{
		SetLocalScale(worldScale);
	}
	else
	{
		Vec3 scale;
		Vec3 parentScale = gameObject->GetParent()->GetTransform()->GetScale();
		scale.x = worldScale.x / parentScale.x;
		scale.y = worldScale.y / parentScale.y;
		scale.z = worldScale.z / parentScale.z;
		SetLocalScale(scale);
	}
}

void Transform::SetRotation(const Vec3& worldRotation)
{
	if (gameObject->GetParent() == nullptr)
	{
		SetLocalRotation(worldRotation);
	}
	else
	{
		Matrix worldToLocalMatrix = gameObject->GetParent()->GetTransform()->GetWorldMatrix().Invert();
		Vec3 rotation;
		Vec3::TransformNormal(worldRotation, worldToLocalMatrix, rotation);
		SetLocalRotation(rotation);
	}
}

void Transform::SetPosition(const Vec3& worldPosition)
{
	if (gameObject->GetParent() == nullptr)
	{
		SetLocalPosition(worldPosition);
	}
	else
	{
		Matrix worldToLocalMatrix = gameObject->GetParent()->GetTransform()->GetWorldMatrix().Invert();
		Vec3 position = Vec3(0.f, 0.f, 0.f);
		Vec3::Transform(worldPosition, worldToLocalMatrix, position);
		SetLocalPosition(position);
	}
}
