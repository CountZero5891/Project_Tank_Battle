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

	Enemy(/*Image& image,*/ AnimationManager &A, String Name, Level& lvl, float X, float Y, int W, int H, int Dir) :Entity(/*image, */A, Name, X, Y, W, H) {
		obj = lvl.GetObjects("solid");//иницализируем объекты с карты
		//isShoot = true;
		direction = rand() % 4 + 0;
		health = 100;
		/*if (name == "Enemy") {
			sprite.setTextureRect(IntRect(8, 1, w, h));
		}*/
	};

	void checkCollisionWithMap(float Dx, float Dy) {

		for (int i = 0; i < obj.size(); i++)//проход по циклу для проверки столкновения врага с препятствием
			if (getRect().intersects(obj[i].rect))
			{
				if (obj[i].name == "solid")
				{
					if (Dy > 0)	{ 
						y = obj[i].rect.top - h;
						direction = 4;
						dy = -0.1; 
					}
					if (Dy < 0) { 
						y = obj[i].rect.top + obj[i].rect.height;   
						direction = 3;
						dy = 0.1; 
					}
					if (Dx > 0) { 
						x = obj[i].rect.left - w;  
						direction = 2;
						dx = -0.1; //dy = 0; 
						
					}
					if (Dx < 0) { 
						x = obj[i].rect.left + obj[i].rect.width;
						direction = 1;
						dx = 0.1; //dy = 0; 
					}
				}
			}
	}

	void update(float time) {
		float timeShoot, timeMove;
		//тут раз в две секунды меняется направление противника
		if (name == "Enemy") {
			Animation(time);
			timeShoot = rand() % 1400 + 1500;
			timeMove = rand() % 2500 + 3000;
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

			if (moveTimer >=timeMove )
			{
				direction = rand() % 4 + 0;
				moveTimer = 0;
			}

			switch (direction)
			{

			case 0:
				dx = -0.1f;
				dy = 0;
				//anim.set("enemy_left");
				//sprite.setRotation(-90);
				break;
			case 1:
				dx = 0.1f;
				dy = 0;
				//anim.set("enemy_right");
				//sprite.setRotation(90);
				break;
			case 2:
				dx = 0;
				dy = -0.1f;
				//anim.set("enemy_up");
				//sprite.setRotation(0);
				break;
			case 3:
				dx = 0;
				dy = 0.1f;
				//anim.set("enemy_down");
				//sprite.setRotation(180);
				break;
			}
			
			checkCollisionWithMap(dx, 0);
			x += dx * time;
			
			checkCollisionWithMap(0, dy);
			y += dy * time;

			sprite.setPosition(x + w / 2.f, y + h / 2.f);
			if (health <= 0)
			{
				life = false;
			}
		}
	}
	
	void Animation(float time)
	{
		//if (STATE == stay) anim.pause();
		if (direction == 2)
		{
			//sprite.setRotation(0);
			anim.set("enemy_up");
			/*anim.pause();
			if (dy != 0)
			{
				anim.play();
			}*/
			//cout << w << endl; cout << h << endl;
		}
		if (direction == 3)
		{
			anim.set("enemy_down");
			
			//cout << w << endl; cout << h << endl;
		}
		if (direction == 0) {
			anim.set("enemy_left");
			
		}
		if (direction == 1)
		{
			anim.set("enemy_right");
			
		}

		anim.tick(time);
	}

};
#endif // !ENEMY_H
