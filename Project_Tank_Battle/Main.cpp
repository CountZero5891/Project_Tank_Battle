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

	Entity(Image &image, String Name, float X, float Y, int W, int H) {
		x = X; y = Y; w = W; h = H; name = Name; moveTimer = 0;
		speed = 0; health = 100; dx = 0; dy = 0;
		life = true; 
		isMove = false;
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
			//sprite.setOrigin(35.4, 35.4);
			sprite.setOrigin(w/2, h/2 );

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
		if (health <= 0)
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
		} 
		if (Keyboard::isKeyPressed(Keyboard::Down))
		{
			state = down;
			speed = 0.1;
		} 
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
	int health;
	bool isShoot;
	
	Enemy(Image& image, String Name, Level& lvl, float X, float Y, int W, int H) :Entity(image, Name, X, Y, W, H) 
	{
		obj = lvl.GetObjects("solid");//инициализируем.получаем нужные объекты для взаимодействия врага с картой
		health = 100;
		if (name == "Enemy") {
			sprite.setTextureRect(IntRect(8, 1, w, h));

		}

	}
	void checkCollisionWithMap(float Dx, float Dy)
	{
		for (int i = 0; i < obj.size(); i++)//проходимся по объектам
			if (getRect().intersects(obj[i].rect))//проверяем пересечение игрока с объектом
			{
				if (obj[i].name == "solid")
				{
					if (Dy > 0) { y = obj[i].rect.top - h;  dy = 0; dx = 0;}
					if (Dy < 0) { y = obj[i].rect.top + obj[i].rect.height;   dy = 0; dx = 0;}
					if (Dx > 0) { x = obj[i].rect.left - w;  dx = 0; dy = 0;/*sprite.scale(-1, 1);*/ }
					if (Dx < 0) { x = obj[i].rect.left + obj[i].rect.width; dx = 0; dy = 0;/*sprite.scale(-1, 1);*/ }

				}
			}
	}


	void checkCollisionWithPlayer(float Dx, float Dy)
	{
		for (int i = 0; i < obj.size(); i++)//проходимся по объектам
			if (getRect().intersects(obj[i].rect))//проверяем пересечение игрока с объектом
			{
				if (obj[i].name == "Player")
				{
					cout << "There you are"<<endl;
				}
			}
	}

	void update(float time)
	{
		if (name == "Enemy") {
			moveTimer += time;

			if (moveTimer > 2500)
			{
				direction = rand() % 4 + 1;
				moveTimer = 0;
			}
			//cout << direction << endl;
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

			checkCollisionWithPlayer(dx, 0);


			isShoot = true;
			sprite.setPosition(x + w / 2, y + h / 2);
			if (health <= 0)
			{
				life = false;
			}
		}
	}
};

class Bullet :public Entity
{
public:
	int direction;

	Bullet(Image& image, String Name, Level& lvl, float X, float Y, int W, int H, int dir) :Entity(image, Name, X, Y, W, H) {
		obj = lvl.GetObjects("solid");
		x = X;
		y = Y;
		direction = dir;
		speed = 0.8;
		w = h = 16;
		life = true;
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

		if (x <= 0) x = 1;
		if (y <= 0) y = 1;

		for (int i = 0; i < obj.size(); i++) {//проход по объектам solid
			if (getRect().intersects(obj[i].rect)) //если этот объект столкнулся с пулей,
			{
				life = false;// то пуля умирает
			}
		}

		sprite.setPosition(x + w / 2, y + h / 2);//задается позицию пуле
	}

};


void exec()
{
	Clock clock;
	float CurrentFrame = 0;
	int createObjectForMapTimer = 0;
	bool showMissionText = true;
	Font font;
	font.loadFromFile("G:/Project_Tank_Battle/fonts/droid-sans.ttf");
	Text text("", font, 20);
	RenderWindow window(VideoMode(1366, 768), "Project_Tank_Battle");
	window.setFramerateLimit(30);
	view.reset(sf::FloatRect(0, 0, 1920, 1080));

	//AnimationManager anim;

	Level lvl;
	lvl.LoadFromFile("map02.tmx");
	//////////////////////////////////////////////////////////////////////////////////////////
	Image heroImage;
	heroImage.loadFromFile("G:/Project_Tank_Battle/images/tanks.png");

	Image EnemyImage;
	EnemyImage.loadFromFile("G:/Project_Tank_Battle/images/tanks.png");

	Object player = lvl.GetObject("Player");//объект игрока на нашей карте.задаем координаты игроку в начале при помощи него
	Player p(heroImage, "Player", lvl, player.rect.left, player.rect.top, 75, 82);

	list<Entity*>  entities;//создаю список, сюда буду кидать объекты.например врагов.
	list<Entity*>::iterator it;//итератор чтобы проходить по эл-там списка
	list<Entity*>::iterator it2;//итератор чтобы проходить по эл-там списка

	vector<Object> e = lvl.GetObjects("Enemy");//все объекты врага на tmx карте хранятся в этом векторе

	Image BulletImage;
	BulletImage.loadFromFile("G:/Project_Tank_Battle/images/bullet.png");
	BulletImage.createMaskFromColor(Color(0, 0, 0));

	for (int i = 0; i < e.size(); i++)//проходимся по элементам этого вектора(а именно по врагам)
	{
		entities.push_back(new Enemy(EnemyImage, "Enemy", lvl, e[i].rect.left, e[i].rect.top, 71, 80));//и закидываем в список всех наших врагов с карты
	}

	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if (p.isShoot == true)
			{
				p.isShoot = false;
				entities.push_back(new Bullet(BulletImage, "Bullet", lvl, p.x, p.y, 16, 16, p.state));
			}//если выстрелили, то появляется пуля. enum передаем как int 
			if (event.type == Event::KeyPressed)//событие нажатия клавиши
			{
				if ((event.key.code == Keyboard::Escape))
				{
					window.close();
					
				}
			}
		}

		float time = clock.getElapsedTime().asMicroseconds();
		clock.restart();
		time = time / 800;

		p.update(time);

		for (it = entities.begin(); it != entities.end();)//говорим что проходимся от начала до конца
		{
			Entity* b = *it;//для удобства, чтобы не писать (*it)->
			b->update(time);//вызываем ф-цию update для всех объектов (по сути для тех, кто жив)
			if (b->life == false) { it = entities.erase(it); delete b; }// если этот объект мертв, то удаляем его
			else it++;//и идем курсором (итератором) к след объекту. так делаем со всеми объектами списка
		}



		for (it = entities.begin(); it != entities.end(); it++)
		{
			if ((*it)->getRect().intersects(p.getRect()))
			{
				if ((*it)->name == "Enemy")
				{
					if ((*it)->dx > 0)
					{
						cout << "(*it)->x" << (*it)->x << "\n";
						cout << "p.x" << p.x << "\n";
						(*it)->x = p.x - (*it)->w;
						(*it)->dx = 0;
						cout << "new(*it)->x" << (*it)->x << "\n";
						cout << "newp.x" << p.x << "\n\n";
					}
					if ((*it)->dx < 0)
					{
						(*it)->x = p.x + p.w;
						(*it)->dx *= 0;
					}
					if (p.dx < 0)
					{
						p.x = (*it)->x + (*it)->w;
					}
					if (p.dx > 0)
					{
						p.x = (*it)->x - p.w;
					}

					if ((*it)->dy > 0)
					{
						(*it)->y = p.y - (*it)->h;
						(*it)->dy = 0;
					}
					if ((*it)->dy < 0)
					{
						(*it)->y = p.y + p.h;
						(*it)->dy = 0;
					}
					if (p.dy < 0)
					{
						p.y = (*it)->y + (*it)->h;
					}
					if (p.dy > 0)
					{
						p.y = (*it)->y - p.h;
					}

				}
			}

			for (it2 = entities.begin(); it2 != entities.end(); it2++)
			{
				if ((*it)->name == "Enemy")
				{
					Entity* enemy = *it;
					for (list<Entity*>::iterator it2 = entities.begin(); it2 != entities.end(); it2++)
					{
						Entity* bullet = *it2;
						if (bullet->name == "Bullet")
						{
							if (bullet->life == true)
							{
								if (bullet->getRect().intersects(enemy->getRect()))
								{
									bullet->life = false;
									enemy->health -= 50;
									if (enemy->health == 0)
									{
										enemy->life = false;
									}
								}
							}
						}
					}
				}
			}



			for (it2 = entities.begin(); it2 != entities.end(); it2++) {
				if ((*it)->getRect() != (*it2)->getRect())//при этом это должны быть разные прямоугольники
					if (((*it)->getRect().intersects((*it2)->getRect())) && ((*it)->name == "Enemy") && ((*it2)->name == "Enemy"))//если столкнулись два объекта и они враги
					{
						(*it)->dx *= -1;//меняем направление движения врага

					}
			}
		}

		window.setView(view);
		window.clear();
		lvl.Draw(window);

		ostringstream playerHealthString, gameTimeString;
		playerHealthString << p.health;
		text.setString("Armor: " + playerHealthString.str());
		text.setPosition(view.getCenter().x - 500, view.getCenter().y - 600);
		for (it = entities.begin(); it != entities.end(); it++)
		{
			window.draw((*it)->sprite); //рисуем entities объекты (сейчас это только враги)
		}
		window.draw(text);

		window.draw(p.sprite);
		window.display();
	}
}

int main()
{
	exec();
	return 0;
}

