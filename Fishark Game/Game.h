#pragma once
#include "UI_Manager.h"
#include "BackGround.h"
#include "Fish.h"
#include "Alga.h"
#include "Shark.h"
#include "Final.h"
/*
Это игра, в ней два уровня: первый уровень - главным героем является рыбка, за которой двигается акула.
Есть другие рыбки, которые главная рыбка может собирать в стаю, если к ним подплывет.
Есть три типа таких рыб - обычная; меняющая цвет, в зависимости от близости акулы, и рыбка, которая уплывает из стаи через некоторое время).
Если главная рыбка находится в стае, то если акула догоняет, в начале ест других рыб (то есть они как защита идут).
Игра заканчивается, когда рыбка доплывает до финиша или когда акула съедает главную рыбку

Второй уровень - главным героем является акула. Она должна дойти до финиша.
У нее есть жизни, которые она пополняет, когда ест рыб (рыбы при приближении акулы должны плыть к водорослям, прятаться), есть другая акула,
которая тоже может поедать этих рыб, поэтому главная акула должна спугнуть, подплыв к ней близко.
Игра заканчивается, когда акула доплывает до финиша или когда заканчиваются жизни, то есть не съела достаточно рыб
*/
enum class GameState { MENU, GAME };

class Game : public B
{
private:

	BackGround bg;
	unique_ptr<Fish> player_fish = std::move(make_unique<FishPlayer>(v2f(cam_p.x - (scr_1h * 42), cam_p.y)));
	unique_ptr<SharkPlayer> player_shark = std::move(make_unique<SharkPlayer>(v2f(cam_p.x - (scr_1h * 42), cam_p.y)));
	vector<FishLeave> vec_fish_leaver;
	vector<FishRecolor> vec_fish_recolor;
	vector<Alga> vec_alga;
	float timer_alga_spawn = 0;
	float timer_fish_spawn = 0;
	GameState game_state = GameState::MENU;
	unique_ptr<Shark> shark_enemy = std::move(make_unique<Shark>(v2f(scr_W / 1.9f, (rand() % scr_H) - (scr_H / 2.f))));
	Final finish;

public:

	void Restart() {
		timer_alga_spawn = 0;
		timer_fish_spawn = 0;
		player_fish = std::move(make_unique<FishPlayer>(v2f(cam_p.x - (scr_1h * 42), cam_p.y)));
		player_shark = std::move(make_unique<SharkPlayer>(v2f(cam_p.x - (scr_1h * 42), cam_p.y)));
		if (level == 1) player_shark->GetShape().setPosition(-5000000, 5000000);
		if (level == 2) player_fish->GetShape().setPosition(-5000000, 5000000);
		shark_enemy = std::move(make_unique<Shark>(v2f(scr_W / 1.9f, (rand() % scr_H) - (scr_H / 2.f))));
		vec_alga.clear();
		vec_fish_leaver.clear();
		vec_fish_recolor.clear();
		finish.Restart();
	}

	Game() : B("init")
	{
		UI_Manager ui_manager;
		while (wnd.isOpen())
		{
			SystemUpdate();
			while (wnd.pollEvent(event)) {
				CloseEvent();
				switch (game_state)
				{
				case GameState::MENU: 
				{
					const string act_id = ui_manager.GetActID();
					if (act_id == "Play") {
						game_state = GameState::GAME;
					}
					else if (act_id == "Exit") {
						wnd.close();
					}
				} 
				break;
				case GameState::GAME:
					if (level == 1) {
						player_fish->Action();
					}
					else {
						player_shark->Action();
					}
					if (IsKeyPressed(Key::Escape)) {
						game_state = GameState::MENU;
						level = 1;
					}
					break;
				default: break;
				}
			}

			switch (game_state)
			{
			case GameState::MENU:
				ui_manager.Update();
				break;
			case GameState::GAME:

				finish.Update();
				if (finish.IsFinish()) {
					if (level == 1) Restart();
					player_shark->GetShape().setPosition(cur_p);
					level = 2;
				}
				if (timer_alga_spawn < 0) {
					timer_alga_spawn = rand() % 360 + 240;
					vec_alga.push_back(Alga(rand() % 3));
				}
				else {
					timer_alga_spawn--;
				}

				if (timer_fish_spawn < 0) {
					timer_fish_spawn = rand() % 200 + 100;
					if (rand() % 2 > 0) vec_fish_leaver.push_back(FishLeave());
					else vec_fish_recolor.push_back(FishRecolor());
				}
				else {
					timer_fish_spawn--;
				}

				for (auto alga = vec_alga.begin(); alga != vec_alga.end();) {
					if (alga->GetIsEnd()) {
						alga = vec_alga.erase(alga);
					}
					else {
						alga->Update();
						alga++;
					}
				}

				for (auto fish_leave = vec_fish_leaver.begin(); fish_leave != vec_fish_leaver.end();) {
					auto& f = *fish_leave;
					// if(Если рыбка ливнула из стаи и уплыла за пределы экрана) // endl
					// или (погибла)
					if ((f.GetState() == FishState::LEAVE && !GetVisible().intersects(f.GetShape().getGlobalBounds())) // endl
						|| f.GetIsAlive() == false) {
						fish_leave = vec_fish_leaver.erase(fish_leave);
					}
					else {
						f.Update();
						if (GetDistance(player_shark->GetShape().getPosition(), f.GetShape().getPosition()) <= 10 * scr_1h) {
							f.DeadTime();
							player_shark->AddHp();
						}
						if (level == 2 && GetDistance(shark_enemy->GetShape().getPosition(), f.GetShape().getPosition()) <= 15 * scr_1h) {
							f.DeadTime();
						}
						for (auto& f2 : vec_fish_leaver) {
							if (f.GetShape().getGlobalBounds().intersects(f2.GetShape().getGlobalBounds())) {
								MoveToAngle(f.GetShape(), GetAngle(f2.GetShape(), f.GetShape()), 3);
								MoveToAngle(f2.GetShape(), GetAngle(f2.GetShape(), f.GetShape()), -3);
							}
						}
						fish_leave++;
					}
				}

				for (auto fish_recolor = vec_fish_recolor.begin(); fish_recolor != vec_fish_recolor.end();) {
					auto& f = *fish_recolor;
					if (f.GetIsAlive()) {
						f.Update();
							if (GetDistance(player_shark->GetShape().getPosition(), f.GetShape().getPosition()) <= 10 * scr_1h) {
								f.DeadTime();
								player_shark->AddHp();
							}
							if (level == 2 && GetDistance(shark_enemy->GetShape().getPosition(), f.GetShape().getPosition()) <= 15 * scr_1h) {
								f.DeadTime();
							}
							for (auto& f2 : vec_fish_recolor) {
								if (f.GetShape().getGlobalBounds().intersects(f2.GetShape().getGlobalBounds())) {
									MoveToAngle(f.GetShape(), GetAngle(f2.GetShape(), f.GetShape()), 3);
									MoveToAngle(f2.GetShape(), GetAngle(f2.GetShape(), f.GetShape()), -3);
								}
							}
						float factor = 1;
						if (GetDistance(f.GetShape(), shark_enemy->GetShape()) < scr_1h * 25) {
							factor = GetDistance(f.GetShape(), shark_enemy->GetShape()) / (scr_1h * 25);
						}
						else factor = 1;
						f.Recolor(factor);
						fish_recolor++;
					}
					else {
						fish_recolor = vec_fish_recolor.erase(fish_recolor);
					}
				}

				shark_enemy->Update();
				if (level == 1) {
					player_shark->GetShape().setPosition(v2f(-5000000, 0));
					player_position = v2f(-50000000,0);
					player_fish->Update();
				}
				else {
					player_fish->GetShape().setPosition(v2f(-5000000, 0));
					player_position = v2f(50000000,0);
					player_shark->Update();
					if (!player_shark->GetIsAlive()) {
						Restart();
					}
				}

				if (shark_enemy->IsAttackDelay()) {
					if (GetDistance(shark_enemy->GetShape(), player_position) < scr_1h * 10) {
						shark_enemy->AttackCooldown();
						bool is_defeat = true;
						for (auto& fish : vec_fish_leaver) {
							if (fish.GetState() == FishState::IN_FOLK) {
								if (level == 1) {
									fish.DeadTime();
									is_defeat = false;
								}
								break;
							}
						}
						if (is_defeat) {
							for (auto& fish : vec_fish_recolor) {
								if (fish.GetState() == FishState::IN_FOLK) {
									fish.DeadTime();
									is_defeat = false;
								}
							}
						}
						if (is_defeat) {
							if(level == 1) Restart();
						}
					}
				}
				bg.Update();
				break;
			default:
				break;
			}
			wnd.clear();
			bg.Draw();
			switch (game_state)
			{
			case GameState::MENU:
				ui_manager.Draw();
				break;
			case GameState::GAME:
				shark_enemy->Draw();
				
				for (auto& f : vec_fish_leaver) {
					f.Draw();
				}
				for (auto& f : vec_fish_recolor) {
					f.Draw();
				}
				for (auto& alga : vec_alga) {
					alga.Draw();
				}
				if (level == 1) {
					player_fish->Draw();
				}
				else {
					player_shark->Draw();
				}
				finish.Draw();
				break;
			default:
				break;
			}
			wnd.display();
		}
	}

	~Game() {

	}
};