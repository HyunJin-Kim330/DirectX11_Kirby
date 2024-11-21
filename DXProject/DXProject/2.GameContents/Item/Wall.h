#pragma once
#include "GameObject.h"

class Wall : public GameObject
{
public: 
	Wall(Vec3 wallPosition);
	~Wall();

	virtual void Init();
	virtual void UpdateObject() override;
	void SetDown(bool val) { down = val; endOfTransform = false; }

private:
	bool down = true;
	bool endOfTransform = true;

private:
	void GoDown();
	void GoUp();

};

