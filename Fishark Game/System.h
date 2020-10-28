#pragma once
#include "Texture.h"
#include "Font.h"

class System
{
public:
	static const sf::FloatRect GetVisible() {
		return sf::FloatRect(cam.getCenter().x - (scr_W / 2), cam.getCenter().y - (scr_H / 2), scr_W, scr_H);
	}
	static void SystemUpdate() {
		cur_p = wnd.mapPixelToCoords(sf::Mouse::getPosition(wnd));
		cur_p_wnd = v2f(sf::Mouse::getPosition(wnd));
		cam_p = cam.getCenter();
	}
	static const v2f GetNormalizedPosition(const v2f& pos, float dist, float angle) {
		const float& A = angle * RAD;
		return pos + v2f(cosf(A), sinf(A)) * dist;
	}
	static const v2f GetNormalizedPosition(Shape& shape, float dist, float angle) {
		const float& A = angle * RAD;
		return shape.getPosition() + v2f(cosf(A), sinf(A)) * dist;
	}
	static const bool IsContains(Shape& shape, const v2f& p) {
		return shape.getGlobalBounds().contains(p);
	}
	static const bool GetCollisionCircle(Shape& c1, Shape& c2) {
		const float r1 = c1.getSize().x / 2.f;
		const float r2 = c2.getSize().x / 2.f;
		const v2f& p1 = c1.getPosition();
		const v2f& p2 = c2.getPosition();
		return GetDistance(p1, p2) > float(r1 + r2) ? false : true;
	}
	static const bool GetCollisionRect(Shape& s1, Shape& s2) {
		return s1.getGlobalBounds().intersects(sf::FloatRect(s2.getPosition().x - (s2.getSize().x / 2.f), s2.getPosition().y - (s2.getSize().y / 2.f), s2.getSize().x, s2.getSize().y));
	}
	static void CollisionUpdate(Shape& static_s1, Shape& dynamic_s2) {
		if (GetCollisionCircle(static_s1, dynamic_s2))
			dynamic_s2.setPosition(GetNormalizedPosition(static_s1.getPosition(), (dynamic_s2.getSize().x + static_s1.getSize().x) / 1.99f, GetAngle(static_s1, dynamic_s2)));
	}

	static void CollisionUpdateNoCondition(Shape& static_s1, Shape& dynamic_s2) {
		dynamic_s2.setPosition(GetNormalizedPosition(static_s1.getPosition(), (dynamic_s2.getSize().x + static_s1.getSize().x) / 1.99f, GetAngle(static_s1, dynamic_s2)));
	}

	static const float GetDistance(const v2f& p1, const v2f& p2) {
		return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
	}
	static const float GetDistance(Shape& s1, Shape& s2) {
		const v2f& p1 = s1.getPosition();
		const v2f& p2 = s2.getPosition();
		return GetDistance(p1, p2);
	}
	static const float GetDistance(Shape& s1, const  v2f& pos) {
		const v2f& p1 = s1.getPosition();
		return GetDistance(p1, pos);
	}

	static const Shape CreateShape(v2f pos, v2f siz, IMG& png) {
		Shape shp;
		shp.setOrigin(siz / 2.f);
		shp.setSize(siz);
		shp.setPosition(pos);
		shp.setTexture(&png);
		return shp;
	}
	static const Shape CreateShape(v2f pos, v2f siz) {
		Shape shp;
		shp.setOrigin(siz / 2.f);
		shp.setSize(siz);
		shp.setPosition(pos);
		return shp;
	}
	static const Shape CreateShape(v2f pos, v2f siz, Color color) {
		Shape shp;
		shp.setSize(siz);
		shp.setOrigin(siz / 2.f);
		shp.setFillColor(color);
		shp.setPosition(pos);
		return shp;
	}
	static const Shape CreateShape(v2f pos, v2f siz, float out_siz, Color color = Color::White, Color out_color = Color::White) {
		Shape shp;
		shp.setSize(siz);
		shp.setOrigin(siz / 2.f);
		shp.setFillColor(color);
		shp.setPosition(pos);
		shp.setOutlineThickness(out_siz);
		shp.setOutlineColor(out_color);
		return shp;
	}

	static const sf::Text CreateText(v2f pos, uint siz, sf::String str, sf::Font& font, Color col) {
		sf::Text text;
		text.setFont(font);
		text.setCharacterSize(siz);
		text.setFillColor(col);
		text.setString(str);
		text.setOrigin(text.getGlobalBounds().width / 2, text.getGlobalBounds().height / 2);
		text.setPosition(pos.x, pos.y - siz/3);
		return text;
	}
	static const float GetAngle(const v2f& p1, const v2f& p2) {
		return atan2f(p2.y - p1.y, p2.x - p1.x) * DEG;
	}
	static const float GetAngle(Shape& shp, const v2f& p) {
		return atan2f(p.y - shp.getPosition().y, p.x - shp.getPosition().x) * DEG;
	}
	static void MoveToAngle(Shape& shape, float angle, float speed) {
		const float& A = angle * RAD;
		shape.move(v2f(cos(A), sin(A)) * speed);
	}
	static const float GetAngle(Shape& shp1, Shape& shp2) {
		const v2f& p1 = shp1.getPosition();
		const v2f& p2 = shp2.getPosition();
		return atan2f(p2.y - p1.y, p2.x - p1.x) * DEG;
	}
	static void CenteringText(sf::Text& text) {
		text.setOrigin(text.getGlobalBounds().width / 2.f, text.getGlobalBounds().height / 1.5f);
	}
	static void CloseEvent() {
		if (event.type == sf::Event::Closed) wnd.close();
	}
	static bool IsKeyPressed(const sf::Keyboard::Key& code) {
		if (event.type == sf::Event::KeyPressed)
			if (event.key.code == code) return true;
		return false;
	} 
	static bool IsKeyReleased(const sf::Keyboard::Key& code) {
		if (event.type == sf::Event::KeyReleased)
			if (event.key.code == code) return true;
		return false;
	}
	static bool IsMouseReleased(const sf::Mouse::Button& code) {
		if (event.type == sf::Event::MouseButtonReleased)
			if (event.key.code == code) return true;
		return false;
	}
	static bool IsMousePressed (const sf::Mouse::Button& code) {
		if (event.type == sf::Event::MouseButtonPressed)
			if (event.key.code == code) return true;
		return false;
	}

	System(void) {}
	System(string init) {
		if (init == "init") 
		{
			scr_W = sf::VideoMode::getDesktopMode().width;
			scr_H = sf::VideoMode::getDesktopMode().height;
			scr_1h = float(scr_H / 100.f);
			font = Font();
			texture = Texture();
			wnd.create(sf::VideoMode(scr_W, scr_H), "Fishark", sf::Style::Default, sf::ContextSettings(0, 0, 8));
			cam.reset(sf::FloatRect(0, 0, scr_W, scr_H));
			cam.setCenter(0, 0);
			cam_p = cam.getCenter();
			cur_p = v2f(0,0);
			cur_p_wnd = v2f(0, 0);
			wnd.setView(cam);
			wnd.setMouseCursorVisible(true);
			wnd.setVerticalSyncEnabled(true);
			level = 1;
			srand(::time(0));
		}
	}

public:
	static sf::RenderWindow wnd;
	static Texture texture;
	static Font font;
	static sf::Event event;
	static sf::View cam;
	static int scr_W;
	static int scr_H;
	static float scr_1h;
	static v2f cam_p;
	static v2f cur_p;
	static v2f cur_p_wnd;
	static v2f player_position;
	static uint level;
};

sf::RenderWindow		B::wnd;
Texture					B::texture;
Font					B::font;
sf::Event				B::event;
sf::View				B::cam;
int						B::scr_W;
int						B::scr_H;
float					B::scr_1h;
v2f						B::cam_p;
v2f						B::cur_p;
v2f						B::cur_p_wnd;
v2f						B::player_position;
uint					B::level;