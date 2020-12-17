#include <SFML/Graphics.hpp>
#include <string>
#include <sstream>
#include <iostream>
#include "level.h"
#include "view.h"
#include <vector>
#include <list>

using namespace sf;
using namespace std;

class Entity
{
public:
	vector <Object> obj;
	float dx, dy, x, y, speed, moveTimer;
	int w, h, health;
	bool life, isMove;
	Texture texture;
	Sprite sprite;
	String name;
	Entity(Image& image, String Name, float X, float Y, int W, int H) {
		x = X; y = Y; w = W; h = H; name = Name; moveTimer = 0;
		speed = 0; health = 100; dx = 0; dy = 0;
		life = true; isMove = false;
		texture.loadFromImage(image);
		sprite.setTexture(texture);
		sprite.setOrigin(w / 2, h / 2);
	}

	FloatRect getRect()
	{	//ф-ция получения прямоугольника. его коорд,размеры (шир,высот).
		return FloatRect(x, y, w, h);//эта ф-ция нужна для проверки столкновений 
	}
	
	virtual void update(float time) = 0;
};


class Player : public Entity
{
public:
	enum { left, right, up, down, stay } state;
	int playerScore, health;
	bool isShoot, life;
	Player(Image& image, String Name, Level& lev, float X, float Y, int W, int H) :Entity(image, Name, X, Y, W, H)
	{
		health = 100, life = true, playerScore = 0; state = stay; obj = lev.GetAllObjects();//инициализируем.получаем все объекты для взаимодействия персонажа с картой
		if (name == "Player")
		{
			sprite.setTextureRect(IntRect(0, 85, w, h));
			sprite.setOrigin(35, 35);
		}
	}

	void update(float time)
	{
		control();
		switch (state)
		{
		case left: dx = -speed; dy = 0; sprite.setRotation(-90);  break;
		case right: dx = speed; dy = 0; sprite.setRotation(90);  break;
		case up: dx = 0; dy = -speed; sprite.setRotation(0);  break;
		case down: dx = 0; dy = speed; sprite.setRotation(180); break;
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
		if (health < 0)
		{
			life = false;
			speed = 0;
		}
	}

	void checkCollisionWithMap(float Dx, float Dy)
	{
		for (int i = 0; i < obj.size(); i++)//проходимся по объектам
			if (getRect().intersects(obj[i].rect))//проверяем пересечение игрока с объектом
			{
				if (obj[i].name == "solid")//если встретили препятствие
				{
					if (Dy > 0)
					{
						y = obj[i].rect.top - h;
						dy = 0;
					}
					if (Dy < 0) {
						y = obj[i].rect.top + obj[i].rect.height;   dy = 0;
						cout << "Есть попадание" << endl;
					}
					if (Dx > 0) { x = obj[i].rect.left - w; }
					if (Dx < 0) { x = obj[i].rect.left + obj[i].rect.width; }
				}
			}
	}



	void control()
	{
		int CurrentFrame = 0;
		if (Keyboard::isKeyPressed(Keyboard::Left))
		{
			state = left;
			speed = 0.1;
		}
		if (Keyboard::isKeyPressed(Keyboard::Right))
		{
			state = right;
			speed = 0.1;
		} //первая координата Х положительна =>идём вправо
		if (Keyboard::isKeyPressed(Keyboard::Up))
		{
			state = up;
			speed = 0.1;
		} //вторая координата (У) отрицательна =>идём вверх (вспоминаем из предыдущих уроков почему именно вверх, а не вниз)
		if (Keyboard::isKeyPressed(Keyboard::Down))
		{
			state = down;
			speed = 0.1;
		} //вторая координата (У) положительна =>идём вниз (если не понятно почему именно вниз - смотрим предыдущие уроки)
	//getplayercoordinateforview(p.getplayercoordinateX(), p.getplayercoordinateY());
		if (Keyboard::isKeyPressed(Keyboard::Space))
		{
			isShoot = true;
		}
	}
};

class Enemy :public Entity
{
public:
	int direction;
	enum { left, right, up, down, stay } state;
	Enemy(Image& image, String Name, Level& lvl, float X, float Y, int W, int H) :Entity(image, Name, X, Y, W, H) {
		obj = lvl.GetObjects("solid");//инициализируем.получаем нужные объекты для взаимодействия врага с картой
		if (name == "Enemy") {
			sprite.setTextureRect(IntRect(8, 1, w, h));
			dx = 0.1;
			dy = 0.1;
		}
	}
	void checkCollisionWithMap(float Dx, float Dy)
	{
		for (int i = 0; i < obj.size(); i++)//проходимся по объектам
			if (getRect().intersects(obj[i].rect))//проверяем пересечение игрока с объектом
			{
				//если встретили препятствие (объект с именем solid)
				if (Dy > 0) { y = obj[i].rect.top - h;  dy = 0; }
				if (Dy < 0) { y = obj[i].rect.top + obj[i].rect.height;   dy = 0.1; }
				if (Dx > 0) { x = obj[i].rect.left - w;  dx = -0.1; sprite.scale(-1, 1); }
				if (Dx < 0) { x = obj[i].rect.left + obj[i].rect.width; dx = 0.1; }

			}
	}

	void update(float time)
	{
		if (name == "Enemy") {
			moveTimer += time;

			if (moveTimer > 3000)
			{
				direction = rand() % 3 + 1;
				//dx *= -1;
				moveTimer = 0;
			}
			cout << "Direction= " << direction << endl;
			switch (direction)
			{
			case 0:
				dx = -0.1;
				dy = 0;
				//checkCollisionWithMap(dx, 0);
				break;
			case 1:
				dx = 0.1;
				dy = 0;
				//checkCollisionWithMap(dx, 0);
				break;
			case 2:
				dx = 0;
				dy = 0.1;
				//checkCollisionWithMap(0, dy);
				break;
			case 3:
				dx = 0;
				dy = -0.1;
				//checkCollisionWithMap(0, dy);
				break;
			}
			checkCollisionWithMap(dx, 0);
			x += dx * time;
			checkCollisionWithMap(0, dy);
			y += dy * time;

			sprite.setPosition(x + w / 2, y + h / 2);
			if (health <= 0)
			{
				life = false;
			}
		}
	}
};

class Bullet : public Entity
{
public:
	int direction;
	Bullet(Image& image, String Name, Level& lvl, float X, float Y, int W, int H, int dir) :Entity(image, Name, X, Y, W, H)
	{
		obj = lvl.GetObjects("solid");
		x = X;
		y = Y;
		direction = dir;
		speed = 0.8;
		w = h = 80;
	}
	void update(float time)
	{
		switch (direction)
		{
		case 0:dx = -speed; dy = 0;
			break;
		case 1:dx = speed; dy = 0;
			break;
		case 2:dx = 0; dy = -speed;
			break;
		case 3:dx = 0; dy = speed;
			break;
		case 4:dx = 0; dy = -speed;
			break;
		case 5:dx = 0; dy = -speed;
			break;
		}
		x += dx * time;
		y += dy * time;

		if (x <= 0)
		{
			x = 1;
		}
		if (y <= 0)
		{
			y = 1;
		}
		sprite.setPosition(x + w / 2, y + h / 2);
	}
};