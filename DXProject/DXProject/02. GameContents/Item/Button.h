#pragma once
#include "00. Engine/GameObject/GameObject.h"
class Wall;

class Button : public GameObject
{
public:
	Button(Vec3 buttonPosition);
	Button(Vec3 buttonPosition, float restoreTime);
	~Button();

	virtual void Init();
	virtual void UpdateObject() override;
	void PushButton();
	Wall* AddWall(Vec3 wallPosition);

private:
	bool pushed = false;
	bool restore = false;
	float restoreTime = 0.f;
	float restoreTimeCalc = 0.f;
	vector<Wall*> wallList;

};

