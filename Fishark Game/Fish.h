#pragma once
#include"Entity.h"

class Fish : public Entity
{
protected:

	Shape shape;
	bool is_alive = true;
	float alpha_rotate = 0;
	bool is_invisibility;

public:

	Fish() {
		is_invisibility = false;
	}

	void SetInvisibility(bool result) {
		is_invisibility = result;
	}

	virtual void Update() override {
		// Do nothing...
	}

	virtual void Action() override {
		// Do nothing...
	}

	virtual void Draw() override {
		wnd.draw(shape);
	}

	virtual void Kill() {
		is_alive = false;
	}

	virtual const bool GetIsAlive() {
		return is_alive;
	}

	virtual void Leave() {
		// Do nothing...
	}

	virtual bool IsLeaveFlock() {
		return false;
	}

	virtual Shape& GetShape() {
		return shape;
	}

	virtual ~Fish() {

	}
};

class FishPlayer : public Fish
{
private:

	bool is_move_up = false;
	bool is_move_down = false;
	
public:

	FishPlayer(v2f position) {
		shape = CreateShape(position, v2f(15, 10) * scr_1h, texture.fish[0]);
	}

	void Update() override {
		player_position = shape.getPosition();
		if (shape.getPosition().y > -(scr_1h * 25.f) && is_move_up)  shape.move(0, -10);
		if (shape.getPosition().y < (scr_1h * 45.f) && is_move_down) shape.move(0, 10);
		if (abs(cur_p.x - shape.getPosition().x) > 10) {
			if (cur_p.x > shape.getPosition().x) shape.move(5, 0);
			else shape.move(-5, 0);
		}
		alpha_rotate+=.123456789;
		shape.setRotation(cos(alpha_rotate) * 10);
	}

	void Action() override {
		if (IsKeyPressed(Key::W) || IsKeyPressed(Key::Up))		is_move_up = true;
		if (IsKeyReleased(Key::W) || IsKeyReleased(Key::Up))	is_move_up = false;
		if (IsKeyPressed(Key::S) || IsKeyPressed(Key::Down))	is_move_down = true;
		if (IsKeyReleased(Key::S) || IsKeyReleased(Key::Down))	is_move_down = false;
	}

	~FishPlayer() override {

	}
};

enum class FishState { MOVE, IN_FOLK, LEAVE, DEAD };

class FishLeave : public Fish
{
protected:

	FishState state;
	float timer_flock_alive = 0;
	
public:

	//static int count_fish_l;
	FishLeave() {
		state = FishState::MOVE;
		shape = CreateShape(v2f(scr_W / 2, -(scr_1h * 15) + rand() % 70 * scr_1h), v2f(10, 5) * scr_1h, texture.fish[2]);
		shape.setScale(-1, 1);
	}

	virtual void Update() override {
		alpha_rotate += .123456789;
		switch (state)
		{
		case FishState::MOVE:
			shape.move(-5, 0);
			if (GetDistance(shape, player_position) < (scr_1h * 10)) {
				JoinFlock();
			}
			break;
		case FishState::IN_FOLK:
			timer_flock_alive--;
			shape.setRotation(GetAngle(player_position, shape.getPosition()));
			if (GetDistance(shape, player_position) > (scr_1h * 12)) 
				MoveToAngle(shape, GetAngle(player_position, shape.getPosition()) + 180, 10);
			if (timer_flock_alive <= 0) {
				Leave();
			}
			break;
		case FishState::LEAVE:
			shape.setRotation(90);
			shape.move(0, -15);
			break;
		case FishState::DEAD:
		{
			v2f scale = v2f(shape.getScale().x + 0.02, shape.getScale().y - 0.02);
			shape.setScale(scale);
			if (shape.getScale().x >= 0) {
				Kill();
			}
		}
			break;
		default:
			break;
		}
		if(state != FishState::MOVE) shape.setRotation(shape.getRotation() + cos(alpha_rotate) * 10);
		else shape.setRotation(cos(alpha_rotate) * 10);
	}

	virtual void Action() override {
		// Do nothing...
	}

	virtual void JoinFlock() {
		if (timer_flock_alive <= 0) {
			state = FishState::IN_FOLK;
			timer_flock_alive = rand() % 600 + 1200;
		}
	}

	virtual void Leave() override{
		state = FishState::LEAVE;
	}

	virtual void DeadTime() {
		state = FishState::DEAD;
	}

	virtual const FishState GetState() {
		return state;
	}

	~FishLeave() override {
	}
};

class FishRecolor : public FishLeave
{
private:

	Color color;
	
public:

	FishRecolor()
	{
		state = FishState::MOVE;
		shape = CreateShape(v2f(scr_W / 2, -(scr_1h * 15) + rand() % 70 * scr_1h), v2f(10, 5) * scr_1h, texture.fish[1]);
		shape.setScale(-1, 1);
	}

	void Update() override {
		alpha_rotate += .123456789;
		switch (state)
		{
		case FishState::MOVE:
			shape.move(-5, 0);
			if (GetDistance(shape, player_position) < (scr_1h * 10)) {
				JoinFlock();
			}
			break;
		case FishState::IN_FOLK:
			timer_flock_alive--;
			shape.setRotation(GetAngle(player_position, shape.getPosition()));
			if (GetDistance(shape, player_position) > (scr_1h * 12))
				MoveToAngle(shape, GetAngle(player_position, shape.getPosition()) + 180, 10);
			break;
		case FishState::LEAVE:
			

			break;
		case FishState::DEAD:
		{
			v2f scale = v2f(shape.getScale().x + 0.02, shape.getScale().y - 0.02);
			shape.setScale(scale);
			if (shape.getScale().x >= 0) {
				Kill();
			}
		}
		break;
		default:
			break;
		}
		if (state != FishState::MOVE) shape.setRotation(shape.getRotation() + cos(alpha_rotate) * 10);
		else shape.setRotation(cos(alpha_rotate) * 10);

	}

	void Recolor(float factor) {
		shape.setFillColor(Color(255 * factor, 255, 255 * factor));
	}

	~FishRecolor() override {

	}
};