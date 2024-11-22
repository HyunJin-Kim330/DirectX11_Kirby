#include "pch.h"
#include "Camera.h"
#include "00. Engine/Manager/SceneManager/SceneManager.h"

Matrix Camera::S_MatView = Matrix::Identity;
Matrix Camera::S_MatProjection = Matrix::Identity;

Camera::Camera() : Super(ComponentType::Camera)
{

}

Camera::~Camera()
{

}

void Camera::Update()
{
	UpdateCameraPosition();
	UpdateMatrix();
}

void Camera::UpdateMatrix()
{
	/*Vec3 eyePosition = GetTransform()->GetPosition();
	Vec3 focusPosition = eyePosition + GetTransform()->GetLook();
	Vec3 upDirection = GetTransform()->GetUp();
	S_MatView = ::XMMatrixLookAtLH(eyePosition, focusPosition, upDirection);*/

	S_MatView = GetTransform()->GetWorldMatrix().Invert();

	if (type == ProjectionType::Perspective)
		S_MatProjection = ::XMMatrixPerspectiveFovLH(XM_PI / 4.f, 800.f / 600.f, 1.f, 100.f);
	else
		S_MatProjection = ::XMMatrixOrthographicLH(13.86, 9.35, 0.f, 1.f);
}

Vec3 Camera::GetPlayerPosition()
{
	return GAME->GetSceneManager()->GetKirbyPosition();
}

void Camera::UpdateCameraPosition()
{
	Vec3 kirbyPosition = GetPlayerPosition();
	Vec3 presentPosition = GetTransform()->GetPosition();

	float x = presentPosition.x;
	float y = presentPosition.y;

	if (cameraX.first < kirbyPosition.x && kirbyPosition.x < cameraX.second)
	{
		x = kirbyPosition.x;
	}

	if (cameraY.first < kirbyPosition.y && kirbyPosition.y < cameraY.second)
	{
		y = kirbyPosition.y;
	}

	GetTransform()->SetPosition(Vec3{ x, y, presentPosition.z });
}