#pragma once
#ifndef PLAYER_H
#define PLAYER_H

#include "Entity.h"
#include "view_cam.h"

class Player: public Entity
{
public:
	enum { left, right, up, down, stay } STATE; // положение игрока
	int playerScore, health;//очки здоровья и счет убитых
	std::map<std::string, bool> key;
	//bool isShoot;
	Player(/*Image& image,*/ AnimationManager& A, String Name, Level& lev, float X, float Y, int W, int H, int Dir) :Entity(/*image,*/ A, Name, X, Y, W, H) {
	
		health = 100, life = true, playerScore = 0;
		STATE = stay;
		obj = lev.GetAllObjects();// инициализируем все объекты на карте чтобы проверить на пересечение игрока с ними
		if (name == "Player")
		{
			/*RectangleShape rectangle(Vector2f(w, h));
			rectangle.setPosition(x + w / 2, y + h / 2);
			*/
			sprite.setTextureRect(IntRect(0, 85, w, h));
			sprite.setOrigin(w / 2.f, h / 2.f);
		}
	};
	
	void update(float time) {
		Keyboard();
		Animation(time);
		//control();//функция управления персонажем
		//switch (STATE)
		//{
		//case left: dx = -speed; dy = 0; sprite.setRotation(-90); cout << h << endl; cout << w << endl;  break;//0
		//case right: dx = speed; dy = 0; sprite.setRotation(90);  cout << h << endl; cout << w << endl; break;//1
		//case up: dx = 0; dy = -speed; sprite.setRotation(0);  cout << h << endl; cout << w << endl; break;//2
		//case down: dx = 0; dy = speed; sprite.setRotation(180); cout << h << endl; cout << w << endl; break;//3
		//}
		sf::Vector2f position = sprite.getPosition();
		//std::cout << "Coordinates of simple sprite: " << position.x << " " << position.y << "\n";
		x += dx * time;
		checkCollisionWithMap(dx * time, 0);
		y += dy * time;
		checkCollisionWithMap(0, dy * time);
		//sprite.setPosition(x + w / 2, y + h / 2);
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
					//cout << "Contact" << endl;
					if (Dy > 0)
					{
						y = (obj[i].rect.top) - h;
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

	void Keyboard()
	{
		if (key["L"])
		{
			/*dir = 1;
			if (STATE != duck) */
			dx = -0.1;
			dy = 0;
			STATE = left;
			/*if (STATE == stay)
			{
				STATE = left;
			}*/
		}

		if (key["R"])
		{
			dx = 0.1;
			dy = 0;
			STATE = right;
			/*if (STATE == stay) 
			{
				STATE = right;
			}*/
		}

		if (key["Up"])
		{ 
			dy = -0.1; 
			dx = 0;
			STATE = up;
			/*if (STATE == stay)
			{
				STATE = up;
			}*/
		}

		if (key["Down"])
		{
			dy = 0.1;
			dx = 0;
			STATE = down;
			/*if (STATE == stay)
			{
				STATE = down;
			}*/
		}

		if (key["Space"])
		{
			isShoot = true;
		}

		/////////////////////если клавиша отпущена///////////////////////////
		if (!(key["L"] || key["R"]))
		{
			dx = 0;
			/*cout << dx << endl;*/
			//cout << "Key left un push, STATE: "<<STATE << endl;
			if (STATE == left || STATE == right) /*STATE = stay;*/anim.pause();
			//if (STATE == right) STATE = stay;
		}

		

		if (!(key["Up"] || key["Down"]))
		{
			dy = 0;
			if (STATE == up || STATE == down) /*STATE = stay;*/anim.pause();
		}

		
		if (!key["Space"])
		{
			isShoot = false;
		}

		key["R"] = key["L"] = key["Up"] = key["Down"] = key["Space"] = false;


		if (Keyboard::isKeyPressed(Keyboard::Left))
			key["L"] = true;
		if (Keyboard::isKeyPressed(Keyboard::Right))
			key["R"] = true;
		if (Keyboard::isKeyPressed(Keyboard::Up))
			key["Up"] = true;
		if (Keyboard::isKeyPressed(Keyboard::Down))
			key["Down"] = true;
		if (Keyboard::isKeyPressed(Keyboard::Space))
			key["Space"] = true;

	}

	void control() {

		if (life == true)
		{
			if (Keyboard::isKeyPressed(Keyboard::Left))
			{
				STATE = left;
				speed = 0.1f;
			}
			if (Keyboard::isKeyPressed(Keyboard::Right))
			{
				STATE = right;
				speed = 0.1f;
			}
			if (Keyboard::isKeyPressed(Keyboard::Up))
			{
				STATE = up;
				speed = 0.1f;
			}
			if (Keyboard::isKeyPressed(Keyboard::Down))
			{
				STATE = down;
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
		if (STATE == stay) anim.pause();
		if (STATE == up)
		{
			sprite.setRotation(0);
			anim.set("up");
			anim.pause();
			if (dy != 0)
			{
				anim.play();
			}
			//cout << w << endl; cout << h << endl;
		}
		if (STATE == down)
		{
			sprite.setRotation(180);
			anim.set("down");
			anim.pause();
			if (dy != 0)
			{
				anim.play();
			}
			//cout << w << endl; cout << h << endl;
		}
		if (STATE == left) { 
			sprite.setRotation(-90);
			anim.set("left"); 
			anim.pause(); 
			//cout << dx << endl;
			if (dx != 0) 
			{ 
				anim.play(); 
			} 
			//cout << w << endl; cout << h << endl;
		}
		if (STATE == right) 
		{ 
			sprite.setRotation(90);
			anim.set("right");
			anim.pause(); 
			//cout << dx << endl;
			if (dx != 0) {
				anim.play();
			}
			//cout << w << endl; cout << h << endl;
		}

		anim.tick(time);
	}

};


#endif // !PLAYER_H

