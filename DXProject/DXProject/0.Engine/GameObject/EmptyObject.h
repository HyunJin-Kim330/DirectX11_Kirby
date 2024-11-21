#pragma once
#include "GameObject.h"
class EmptyObject : public GameObject
{
public:
	EmptyObject();
	~EmptyObject();

private:

protected:
	virtual void UpdateObject() override;

};

