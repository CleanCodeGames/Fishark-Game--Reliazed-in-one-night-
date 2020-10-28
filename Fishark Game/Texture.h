#pragma once
#include"Define.h"

class Texture
{
private:
	IMG Load(const string file) {
		IMG texture;
		texture.loadFromFile("res/images/" + file);
		texture.setSmooth(true);
		return texture;
	}

public:
	IMG water;
	IMG seabed;
	IMG coral;
	IMG buble;
	IMG finish;
	IMG fish[3];
	IMG shark[3];
	IMG alga[3];

	Texture() {
		for (short i = 0; i < 3; i++) {
			const short n = i + 1;
			fish[i]  = Load("fish" + to_string(n) + ".png");
			shark[i] = Load("shark" + to_string(n) + ".png");
			alga[i]  = Load("alga" + to_string(n) + ".png");
		}

		water  = Load("water.png");
		seabed = Load("seabed.png");
		coral  = Load("coral.png");
		buble  = Load("buble.png");
		finish = Load("finish.png");
	}

	~Texture(void) {
	}
};
