#pragma once
#ifndef ENEMY_H
#define ENEMY_H

#include "Entity.h"
class Enemy : public Entity
{
public:
	int direction;
	int health;
	//bool isShoot;
	float shootTimer;

	Enemy(Image& image, String Name, Level& lvl, float X, float Y, int W, int H, int Dir) :Entity(image, Name, X, Y, W, H, Dir) {
		obj = lvl.GetObjects("solid");//иницализируем объекты с карты
		//isShoot = true;
		direction = Dir;
		health = 100;
		if (name == "Enemy") {
			sprite.setTextureRect(IntRect(8, 1, w, h));
		}
	};

	void checkCollisionWithMap(float Dx, float Dy) {
		for (int i = 0; i < obj.size(); i++)//проход по циклу для проверки столкновения врага с препятствием
			if (getRect().intersects(obj[i].rect))
			{
				if (obj[i].name == "solid")
				{
					if (Dy > 0) { y = obj[i].rect.top - h;  dy = 0; }
					if (Dy < 0) { y = obj[i].rect.top + obj[i].rect.height;   dy = 0; }
					if (Dx > 0) { x = obj[i].rect.left - w;  dx = 0; dy = 0; }
					if (Dx < 0) { x = obj[i].rect.left + obj[i].rect.width; dx = 0; dy = 0; }
				}
			}
	}



	void update(float time) {
		float timeShoot;
		//тут раз в две секунды меняется направление противника
		if (name == "Enemy") {

			timeShoot = rand() % 1400 + 1500;
			moveTimer += time;
			shootTimer += time;
			if (isShoot == false)
			{
				if (shootTimer >= timeShoot)
				{
					isShoot = true;
					shootTimer = 0;
					timeShoot = 0;
				}
			}


			if (moveTimer > 2500)
			{
				direction = rand() % 4 + 1;
				moveTimer = 0;
			}


			switch (direction)
			{
			case 1:
				dx = 0.1;
				dy = 0;
				sprite.setRotation(90);
				break;
			case 2:
				dx = -0.1;
				dy = 0;
				sprite.setRotation(-90);
				break;
			case 3:
				dx = 0;
				dy = 0.1;
				sprite.setRotation(180);
				break;
			case 4:
				dx = 0;
				dy = -0.1;
				sprite.setRotation(0);
				break;
			}
			x += dx * time;
			checkCollisionWithMap(dx, 0);

			y += dy * time;
			checkCollisionWithMap(0, dy);

			sprite.setPosition(x + w / 2, y + h / 2);
			if (health <= 0)
			{
				life = false;
			}
		}
	}
	

};
#endif // !ENEMY_H
