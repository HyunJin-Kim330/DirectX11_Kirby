#pragma once
#include "Component.h"

enum class ProjectionType
{
	Perspective, // 원근 투영
	Orthographic, // 직교 투영
};

class Camera : public Component
{
	using Super = Component;
public:
	Camera();
	virtual ~Camera();

	virtual void Update() override;

	void UpdateMatrix();
	ProjectionType GetProjectionType() { return type; }
	void SetProjectionType(ProjectionType _type) { type = _type; }
	void SetCameraMovePosition(pair<float, float> x, pair<float, float> y) { cameraX = x; cameraY = y; }

private:
	ProjectionType type = ProjectionType::Orthographic;
	pair<float, float> cameraX = { 0.f, 0.f };
	pair<float, float> cameraY = { 0.f, 0.f };

public:
	static Matrix S_MatView;
	static Matrix S_MatProjection;

private:
	Vec3 GetPlayerPosition();
	void UpdateCameraPosition();
};

