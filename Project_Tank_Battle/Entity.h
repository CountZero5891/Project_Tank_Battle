#pragma once

#ifndef ENTITY_H
#define ENTITY_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <vector>
#include <list>
#include "level.h"
#include "Animation.h"

using namespace std;
using namespace sf;

class Entity
{
public:
	vector <Object> obj;
	AnimationManager anim;
	float dx, dy, x, y, speed, moveTimer;
	int w, h, health;
	bool life, isMove, isShoot;
	//int dirr;

	Texture texture;
	Sprite sprite;
	String name;
	Entity(Image& image, AnimationManager &A, String Name, float X, float Y, int W, int H/*, int Dir*/) {
		x = X; 
		y = Y; 
		w = W; 
		h = H; 
		name = Name; 
		moveTimer = 0;
		speed = 0; 
		health = 100; 
		dx = 0; 
		dy = 0;
		life = true;
		isMove = false;
		//dirr = Dir;
		isShoot = true;
		texture.loadFromImage(image);
		anim = A;
		sprite.setTexture(texture);
		sprite.setOrigin(w / 2, h / 2);
	}
	
	void draw(RenderWindow& window)
	{
		anim.draw(window, x, y + h);
	}

	FloatRect getRect() {
		return FloatRect(x, y, w, h);
	}

	void option(std::string NAME, float SPEED = 0, int HEALTH = 10, std::string FIRST_ANIM = "")
	{
		name = NAME;
		if (FIRST_ANIM != "") anim.set(FIRST_ANIM);
		w = anim.getW();
		h = anim.getH();
		dx = SPEED;
		health = HEALTH;
	}

	virtual void update(float time) = 0;
};




#endif