#pragma once
#include "System.h"

class Entity : public B
{
protected:

public:

	Entity() {
	}
	virtual void Update() = 0;
	virtual void Action() = 0;
	virtual void Draw() = 0;
	virtual ~Entity() = default;
};