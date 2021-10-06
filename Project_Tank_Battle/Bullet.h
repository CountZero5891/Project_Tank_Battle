#pragma once
#ifndef BULLET_H
#define BULLET_H

#include "Entity.h"

class Bullet :public Entity
{
public:
	int direction;

	Bullet(/*Image& image,*/ AnimationManager &A, String Name, Level& lvl, float X, float Y, int W, int H, int dir) :Entity(/*image,*/ A, Name, X, Y, W, H) {
		obj = lvl.GetObjects("solid");
		x = X;
		y = Y;
		direction = dir;
		speed = 0.8;
		w = W;
		h =	H;
		life = true;
	};

	void update(float time) {
		switch (direction)
		{
		case 0:
			dx = -speed;
			dy = 0;
			break;
		case 1:
			dx = speed;
			dy = 0;
			break;
		case 2:
			dx = 0; 
			dy = -speed;
			break;
		case 3:
			dx = 0; 
			dy = speed;
			break;
		/*case 4:
			dx = 0;
			dy = -speed;
			break;*/
		}


		x += dx * time;
		y += dy * time;

		if (x <= 0) x = 1;
		if (y <= 0) y = 1;

		for (int i = 0; i < obj.size(); i++) {
			if (getRect().intersects(obj[i].rect)) // если пуля попала в стену то убираем ее
			{
				life = false;
			}
		}

		sprite.setPosition(x + w / 2, y + h / 2);
	}
	
};

#endif
