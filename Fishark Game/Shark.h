#pragma once
#include "Entity.h"

class Shark : public Entity
{
private:

	v2f order_position;
	float timer_order_wait;
	float attack_cooldown;

protected:

	Shape shape;

public:

	Shark(v2f position) {
		attack_cooldown = 60;
		timer_order_wait = 0;
		shape = CreateShape(position, v2f(25, 10) * scr_1h, texture.shark[0]);
		order_position = v2f(-(scr_W / 2.f), (rand() % scr_H) - (scr_H / 2.f));
		shape.setRotation(GetAngle(shape, order_position));
	}

	virtual void Update() override {
		attack_cooldown--;
		if (GetDistance(shape, order_position) > scr_1h * 5) {
			MoveToAngle(shape, GetAngle(shape, order_position), 25);
		}
		else if (timer_order_wait > 0) {
			timer_order_wait--;
			shape.rotate(timer_order_wait);
		}
		else {
			timer_order_wait = 60;
			if (order_position.x >= 0) {
				order_position = v2f(-(scr_W / 2.f), (rand() % scr_H) - (scr_H / 2.f));
				shape.setScale(1, -1);
			}
			else {
				order_position = v2f(scr_W / 2.15f, (rand() % scr_H) - (scr_H / 2.f));
				shape.setScale(1, 1);
			}
			shape.setRotation(GetAngle(shape, order_position));
		}
	}

	virtual void AttackCooldown() {
		attack_cooldown = 60;
	}

	virtual bool IsAttackDelay() {
		return attack_cooldown <= 0;
	}

	virtual void Action() override {
		// Do nothing...
	}

	virtual void Draw() override {
		wnd.draw(shape);
	}

	virtual Shape& GetShape() {
		return shape;
	}

	virtual ~Shark() override {

	}
};

class SharkPlayer : public Shark
{
private:

	float timer_hp;
	int hp = 10;
	int HP = 275;
	int STEP = 120;
	float step_cooldown;
	float timer_add_hp_cooldown = 0;
	v2f step_point;
	vector<Shape> vec_shape_hp;

public:

	SharkPlayer(v2f position) : Shark(position) {
		shape = CreateShape(position, v2f(25, 10) * scr_1h, texture.shark[1]);
		SetTimerHp(HP);
		SetStepCooldown(STEP);
	}

	virtual void Update() override {
		timer_add_hp_cooldown--;
		if (GetDistance(shape, step_point) > 50)
			MoveToAngle(shape, GetAngle(shape, step_point), 30);
		if (step_cooldown > 0)
		{
			shape.rotate(step_cooldown);
		}
		step_cooldown--;
		timer_hp--;
		if (timer_hp < 0)
		{
			timer_hp = HP;
			hp--;
			if (hp > 0)
			{
				cout << "hp deleted\n";
				if (!vec_shape_hp.empty()) {
					vec_shape_hp.pop_back();
				}
			}
		}
	}

	void Draw() override {
		wnd.draw(shape);
		for (auto& hit : vec_shape_hp) {
			wnd.draw(hit);
		}
	}

	void SetStepCooldown(int result) {
		STEP = result;
	}

	virtual void Action() override {
		if(IsMousePressed(sf::Mouse::Left)) {
			if (step_cooldown <= 0) {
				step_point = cur_p;
				step_cooldown = STEP;
				shape.setRotation(GetAngle(shape, step_point));
			}
		}
	}

	virtual ~SharkPlayer() override {

	}

	void AddHp() {
		if (timer_add_hp_cooldown <= 0)
		{
			hp++;
			v2f pos = vec_shape_hp.back().getPosition();
			pos.x += 64;
			vec_shape_hp.push_back(CreateShape(pos, v2f(44, 44), -2, Color::Red, Color::Black));
			timer_add_hp_cooldown = 300;
		}
	}

	void SetTimerHp(int timer)
	{
		vec_shape_hp.clear();
		for (int i = 0; i < hp; i++) {
			vec_shape_hp.push_back(CreateShape(v2f(i * 64, 44) - v2f(scr_W/2, scr_H/2 + 46), v2f(44, 44), -2, Color::Red, Color::Black));
		}
		HP = timer;
	}

	bool GetIsAlive() {
		return hp > 0;
	}
};