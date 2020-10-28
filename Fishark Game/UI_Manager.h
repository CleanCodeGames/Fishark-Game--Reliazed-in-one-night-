#pragma once
#include "Button.h"

class UI_Manager : public B
{
private:

	vector<ButtonClickActiveWithText> vec_button;
	Shape logo_shark[2];
	float alpha_wave_animation_shark = 0.f;
	sf::Text text_game_name;

public:

	UI_Manager() {
		vec_button.push_back(ButtonClickActiveWithText(CreateShape(v2f(0, -5) * scr_1h, v2f(25, 7.5) * scr_1h, -2, Color(80,80,140), Color(40,40,40)), "Play"));
		vec_button.push_back(ButtonClickActiveWithText(CreateShape(v2f(0,  5) * scr_1h, v2f(25, 7.5) * scr_1h, - 2, Color(80, 80, 140), Color(40, 40, 40)), "Exit"));
		logo_shark[0] = CreateShape(v2f(-40, 0) * scr_1h, v2f(25, 25) * scr_1h, texture.shark[2]);
		logo_shark[1] = CreateShape(v2f( 40, 0) * scr_1h, v2f(25, 25) * scr_1h, texture.shark[2]);
		logo_shark[0].setScale(-1, 1);
		text_game_name = CreateText(v2f(0, -12 * scr_1h), scr_1h * 6, L"Fishark Game! :)", font.super_mario, Color::Magenta);
	}

	void Update() {
		alpha_wave_animation_shark += .1f;
		for (auto& b : vec_button) {
			b.Update();
		}
		alpha_wave_animation_shark += .03789;
		logo_shark[0].setRotation(sin(alpha_wave_animation_shark) * 10);
		logo_shark[0].setPosition(-40 * scr_1h, cos(alpha_wave_animation_shark) * 15 * scr_1h);
		logo_shark[1].setRotation(cos(alpha_wave_animation_shark) * 10); 
		logo_shark[1].setPosition( 40 * scr_1h, sin(alpha_wave_animation_shark) * 15 * scr_1h);
	}
	
	string GetActID() {
		for (auto& b : vec_button) {
			if (b.Action()) {
				return b.GetActionId();
			}
		}
		return "";
	}

	void Draw() {
		for (auto& logo : logo_shark) {
			wnd.draw(logo);
		}
		for (auto& b : vec_button) {
			b.Draw();
		}
		wnd.draw(text_game_name);
	}
};