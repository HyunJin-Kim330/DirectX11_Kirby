#pragma once
#include "00. Engine/GameObject/GameObject.h"

class EmptyObject : public GameObject
{
public:
	EmptyObject();
	~EmptyObject();

private:

protected:
	virtual void UpdateObject() override;

};

