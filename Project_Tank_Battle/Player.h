#pragma once
#ifndef PLAYER_H
#define PLAYER_H

#include "Entity.h"
#include "view_cam.h"

class Player: public Entity
{
public:
	enum { left, right, up, down, stay } state; // положение игрока
	int playerScore, health;//очки здоровья и счет убитых
	//bool isShoot;
	Player(Image& image, String Name, Level& lev, float X, float Y, int W, int H, int Dir) :Entity(image, Name, X, Y, W, H) {
	
		health = 100, life = true, playerScore = 0;
		state = stay;
		obj = lev.GetAllObjects();// инициализируем все объекты на карте чтобы проверить на пересечение игрока с ними
		if (name == "Player")
		{
			sprite.setTextureRect(IntRect(0, 85, w, h));
			sprite.setOrigin(w / 2.f, h / 2.f);
		}
	};
	
	void update(float time) {
		control();//функция управления персонажем
		//Animation(time);
		switch (state)
		{
		case left: dx = -speed; dy = 0; sprite.setRotation(-90);  break;//0
		case right: dx = speed; dy = 0; sprite.setRotation(90);  break;//1
		case up: dx = 0; dy = -speed; sprite.setRotation(0);  break;//2
		case down: dx = 0; dy = speed; sprite.setRotation(180); break;//3
		}
		x += dx * time;
		checkCollisionWithMap(dx * time, 0);
		y += dy * time;
		checkCollisionWithMap(0, dy * time);
		sprite.setPosition(x + w / 5, y + h / 2);
		if (!isMove)
		{
			speed = 0;
		}
		if (life)
		{
			setPlayerCoordinateForView(x, y);
		}
		if (health <= 0)
		{
			life = false;
			speed = 0;
		}
	}

	void checkCollisionWithMap(float Dx, float Dy) {
		for (int i = 0; i < obj.size(); i++)//проход по циклу для проверки столкновения игрока со стеной
		{
			if (getRect().intersects(obj[i].rect))
			{
				if (obj[i].name == "solid")
				{
					if (Dy > 0)
					{
						y = obj[i].rect.top - h;
						dy = 0;
					}
					if (Dy < 0) {
						y = obj[i].rect.top + obj[i].rect.height;
						dy = 0;
					}
					if (Dx > 0)
					{
						x = obj[i].rect.left - w;
					}
					if (Dx < 0)
					{
						x = obj[i].rect.left + obj[i].rect.width;
					}
				}
			}
		}
	}

	void control() {

		if (life == true)
		{
			if (Keyboard::isKeyPressed(Keyboard::Left))
			{
				state = left;
				speed = 0.1f;
			}
			if (Keyboard::isKeyPressed(Keyboard::Right))
			{
				state = right;
				speed = 0.1f;
			}
			if (Keyboard::isKeyPressed(Keyboard::Up))
			{
				state = up;
				speed = 0.1f;
			}
			if (Keyboard::isKeyPressed(Keyboard::Down))
			{
				state = down;
				speed = 0.1f;
			}

			if (Keyboard::isKeyPressed(Keyboard::Space))
			{
				isShoot = true;
			}
		}
	}

	void Animation(float time)
	{
		if (state == stay) anim.set("stay");
		if (state == up) anim.set("up");
		if (state == down) anim.set("down");
		if (state == left) anim.set("left");
		if (state == right) { anim.set("right"); 
		anim.pause(); if (dy != 0) anim.play(); }

		anim.tick(time);
	}

};


#endif // !PLAYER_H

