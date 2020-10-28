#pragma once
#include "System.h"

class Final : public B
{
private:

	Shape shape_progress_bar[2];
	Shape shape_finish;
	float timer_to_finish;
	float timer = 0;
	bool is_show_finish = false;

public:

	Final(float timer_to_finish = 5000) : timer_to_finish(timer_to_finish) {
		shape_progress_bar[0] = CreateShape(v2f(-scr_W / 2, -(scr_H / 2) + 25), v2f(scr_W, 6), Color::Cyan);
		shape_progress_bar[0].setOrigin(0, 0);
		shape_progress_bar[1] = CreateShape(v2f(-scr_W / 2, -(scr_H / 2) + 25), v2f(0, 6), Color::Green);
		shape_progress_bar[1].setOrigin(0, 0);

		shape_finish = CreateShape(v2f(), v2f(50, 50) * scr_1h, texture.finish);
	}

	void Update() {
		if (timer >= timer_to_finish) {
			is_show_finish = true;
		}
		else {
			timer++;
			shape_progress_bar[1].setSize(v2f(timer / timer_to_finish * scr_W, 6));
		}
	}

	bool const IsFinish() {
		return is_show_finish;
	}

	void Draw() {
		if (is_show_finish) {
			wnd.draw(shape_finish);
		}
		wnd.draw(shape_progress_bar[0]);
		wnd.draw(shape_progress_bar[1]);
	}

	void Restart() {
		timer = 0;
		is_show_finish = false;
	}

	~Final() {

	}
};