#pragma once
#include "00. Engine/GameObject/GameObject.h"

class Background : public GameObject
{
public:
	Background();
	~Background();

	void SetFollowX(pair<float, float> _followX) { followX = _followX; }
	void SetFollowY(pair<float, float> _followY) { followY = _followY; }

private:
	Vec3 firstKirbyPosition = Vec3{ 0.f, 0.f, 0.f };
	pair<float, float> followX = { 0.f, 0.f };
	pair<float, float> followY = { 0.f, 0.f };

private:
	virtual void UpdateObject() override;
};

