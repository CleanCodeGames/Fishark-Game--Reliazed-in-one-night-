#pragma once
#include "Buble.h"

class BackGround : public Entity
{
private:

	list<Shape> list_shape_water;
	list<Shape> list_shape_seabed;
	vector<unique_ptr<Buble>> vec_buble;
	float timer_buble_spawn = 0;

public:

	BackGround() {
		list_shape_water.push_back(CreateShape(v2f(-scr_W / 2, 0), v2f(scr_W, scr_H), texture.water));
		list_shape_water.push_back(CreateShape(v2f( scr_W / 2, 0), v2f(scr_W, scr_H), texture.water));
		list_shape_water.back().setScale(-1, 1);
		list_shape_seabed.push_back(CreateShape(v2f(-scr_W / 2, scr_H * .25), v2f(scr_W, scr_H / 2.f), texture.seabed));
		list_shape_seabed.push_back(CreateShape(v2f( scr_W / 2, scr_H * .25), v2f(scr_W, scr_H / 2.f), texture.seabed));
		list_shape_seabed.back().setScale(-1, 1);
	}

	void Update() override {
		if (timer_buble_spawn < 0) {
			timer_buble_spawn = rand() % 60 + 45;
			vec_buble.push_back(make_unique<Buble>(v2f(rand() % scr_W - scr_W / 2, scr_H / 2.f)));
		}
		else {
			timer_buble_spawn--;
		}

		for (auto buble = vec_buble.begin(); buble != vec_buble.end();) {
			auto& b = **buble;
			if (b.IsBurst()) {
				const v2f position = b.GetPosition();
				const BubleType buble_type = b.GetType();
				buble = vec_buble.erase(buble);
				if (buble_type == BubleType::BIG) {
					const short buble_mini_nums = rand() % 3 + 5;
					for (auto i = 0; i < buble_mini_nums; i++) {
						vec_buble.push_back(make_unique<BubleSmall>(v2f(GetNormalizedPosition(position, scr_1h * (rand() % 3 + 1), rand() % 360))));
					}
					break;
				}
				else continue;
			}
			else {
				b.Update();
				buble++;
			}
		}

		for (auto& water : list_shape_water) {
			if (water.getPosition().x < -scr_W) {
				water.move(scr_W * 2, 0);
			}
			water.move(-8, 0);
		}
		for (auto& seabed : list_shape_seabed) {
			if (seabed.getPosition().x < -scr_W) {
				seabed.move(scr_W * 2, 0);
			}
			seabed.move(-5, 0);
		}
	}

	void Action() override {
		// Do nothing...
	}

	void Draw() override {
		for (auto& water : list_shape_water) {
			wnd.draw(water);
		}
		for (auto& buble : vec_buble) {
			buble->Draw();
		}
		for (auto& seabed : list_shape_seabed) {
			wnd.draw(seabed);
		}
	}

	~BackGround() override {
	}
};