#pragma once
#include "Entity.h"

class Alga : public Entity
{
private:

	Shape shape;
	float speed;

public:

	// type 0-2 max
	Alga(int type) {
		shape = CreateShape(v2f(scr_W / 1.9f, -(scr_1h * 15) + rand()%70*scr_1h), v2f(36, 52) * scr_1h, texture.alga[type]);
		speed = .2 * int(rand() % 5 + 5);
	}
	
	const Shape GetShape() {
		return shape;
	}

	void Update() override {
		shape.move(-speed, 0);
	}

	void Action() override {
		// Do nothing...
	}

	const bool GetIsEnd() {
		return shape.getPosition().x < -(scr_W / 1.9f);
	}

	void Draw() override {
		wnd.draw(shape);
	}

	~Alga() override {

	}
};