#pragma once
#include "00. Engine/GameObject/GameObject.h"

class Door : public GameObject
{
public:
	Door(string _sceneName, Vec3 _kirbyInitPosition, Vec3 _cameraInitPosition);
	~Door();

	string GetSceneName() const { return sceneName; };
	Vec3 GetKirbyInitPosition() const { return kirbyInitPosition; }

protected:
	virtual void UpdateObject() override;

private:
	string sceneName;
	Vec3 kirbyInitPosition;
	Vec3 cameraInitPosition;
};

