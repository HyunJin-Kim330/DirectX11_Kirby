#pragma once
#include "Types.h"

struct CameraData
{
	Matrix matView = Matrix::Identity;
	Matrix matProjection = Matrix::Identity;
};

struct TransformData
{
	Matrix matWorld = Matrix::Identity;
};

struct AnimationData
{
	Vec2 offset;
	Vec2 size;
	Vec2 textureSize;
	float useAnimation;
	float padding;
};

struct CollisionRect
{

};