#pragma once
#include "Entity.h"

enum class BubleType { BIG, MINI };

class Buble : public B
{
protected:

	BubleType type;
	Shape shape;
	bool is_burst = false;
	float time_of_life;

public:

	Buble(v2f position) {
		time_of_life = rand() % 300 + 200;
		shape = CreateShape(position, v2f(scr_1h, scr_1h) * float(rand() % 3 + 5), texture.buble);
		type = BubleType::BIG;
	}

	virtual void Update() {
		time_of_life -= 1;
		if (time_of_life <= 0 || shape.getPosition().y < -scr_H * .15) {
			is_burst = true;
		}
		shape.move(-1, -1.76);
	}

	virtual void Draw() {
		wnd.draw(shape);
	}

	virtual void Burst() {
		is_burst = true;
	}

	virtual const bool IsBurst() {
		return is_burst;
	}

	virtual const v2f& GetPosition() {
		return shape.getPosition();
	}

	virtual const BubleType GetType() {
		return type;
	}

	~Buble() {
	}
};

class BubleSmall : public Buble
{
public:

	BubleSmall(v2f position) : Buble(position) {
		time_of_life = 240;
		shape = CreateShape(position, v2f(scr_1h / 3, scr_1h / 3) * float(rand() % 3 + 3), texture.buble);
		type = BubleType::MINI;
	}

	~BubleSmall() {
	}
};