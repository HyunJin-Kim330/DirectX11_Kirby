#pragma once

class Foreground : public GameObject
{
public:
	Foreground();
	~Foreground() {}

private:
	Vec3 firstKirbyPosition = Vec3{ 0.f, 0.f, 0.f };


private:
	virtual void UpdateObject() override;
};

