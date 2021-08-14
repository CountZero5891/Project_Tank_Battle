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
	bool life, isMove, isShoot;
	int dirr;

	Texture texture;
	Sprite sprite;
	String name;
	Entity(Image& image, String Name, float X, float Y, int W, int H, int Dir) {
		x = X; y = Y; w = W; h = H; name = Name; moveTimer = 0;
		speed = 0; health = 100; dx = 0; dy = 0;
		life = true;
		isMove = false;
		dirr = Dir;
		//isShoot = true;
		texture.loadFromImage(image);
		sprite.setTexture(texture);
		sprite.setOrigin(w / 2, h / 2);


	}
	FloatRect getRect() {
		//Функция для получения данных о прямоугольнике(координаты положения и его размер)
		return FloatRect(x, y, w, h);
	}

	virtual void update(float time) = 0;
};

class Player : public Entity
{
public:
	enum { left, right, up, down, stay } state; // положение игрока
	int playerScore, health;//очки здоровья и счет убитых
	//bool isShoot;
	Player(Image& image, String Name, Level& lev, float X, float Y, int W, int H, int Dir) :Entity(image, Name, X, Y, W, H, Dir)
	{
		health = 100, life = true, playerScore = 0;
		state = stay;
		obj = lev.GetAllObjects();// инициализируем все объекты на карте чтобы проверить на пересечение игрока с ними
		if (name == "Player")
		{
			sprite.setTextureRect(IntRect(0, 85, w, h));
			sprite.setOrigin(w / 2, h / 2);
		}
	}

	void update(float time)
	{
		control();//функция управления персонажем
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
		for (int i = 0; i < obj.size(); i++)//проход по циклу для проверки столкновения игрока со стеной
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


	//функция управления игроком
	void control()
	{
		
		if (Keyboard::isKeyPressed(Keyboard::Left))
		{
			state = left;
			speed = 0.1;
		}
		if (Keyboard::isKeyPressed(Keyboard::Right))
		{
			state = right;
			speed = 0.1;
		}
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
	//bool isShoot;
	float shootTimer;

	Enemy(Image& image, String Name, Level& lvl, float X, float Y, int W, int H, int Dir) :Entity(image, Name, X, Y, W, H, Dir)
	{
		obj = lvl.GetObjects("solid");//иницализируем объекты с карты
		//isShoot = true;
		direction = Dir;
		health = 100;
		if (name == "Enemy") {
			sprite.setTextureRect(IntRect(8, 1, w, h));
		}

	}

	void checkCollisionWithMap(float Dx, float Dy)
	{
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


	void update(float time)
	{
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

class Bullet :public Entity
{
public:
	int direction;

	Bullet(Image& image, String Name, Level& lvl, float X, float Y, int W, int H, int dir) :Entity(image, Name, X, Y, W, H, dir) {
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

		for (int i = 0; i < obj.size(); i++) {
			if (getRect().intersects(obj[i].rect)) // если пуля попала в стену то убираем ее
			{
				life = false;
			}
		}

		sprite.setPosition(x + w / 2, y + h / 2);

	}
};


void CollisionBetweenObject()
{
	//Заготовка для функции которая проверяет взаимодействия между объектами
}


void exec()
{
	Clock clock;
	float CurrentFrame = 0;
	Font font;
	font.loadFromFile("fonts/droid-sans.ttf");
	Text text("", font, 20);
	RenderWindow window(VideoMode(1366, 768), "Project_Tank_Battle");
	window.setFramerateLimit(30);
	view.reset(sf::FloatRect(0, 0, 1920, 1080));


	/////инициализация объектов класса и взятие спрайтов для игрока и противников
	Level lvl;
	lvl.LoadFromFile("map02.tmx");

	Image heroImage;
	heroImage.loadFromFile("images/tanks.png");

	Image EnemyImage;
	EnemyImage.loadFromFile("images/tanks.png");

	Object player = lvl.GetObject("Player");
	Player p(heroImage, "Player", lvl, player.rect.left, player.rect.top, 75, 82, 1);



	list<Entity*>  entities;//создаю список, сюда буду кидать объекты
	list<Entity*>::iterator it;//итератор чтобы проходить по эл-там списка

	vector<Object> e = lvl.GetObjects("Enemy");//все объекты врага на tmx карте хранятся в этом векторе

	list<Enemy*> enemies;//список противников

	//vector<Enemy> enemy;

	Image BulletImage;
	BulletImage.loadFromFile("images/bullet.png");
	BulletImage.createMaskFromColor(Color(0, 0, 0));

	for (int i = 0; i < e.size(); i++)//проходимся по элементам этого вектора(а именно по врагам)
	{
		entities.push_back(new Enemy(EnemyImage, "Enemy", lvl, e[i].rect.left, e[i].rect.top, 71, 80, 1));
	}

	for (it = entities.begin(); it != entities.end(); ++it) {
		cout << (*it)->x << " " << (*it)->y << " " << (*it)->dirr << endl;
	}

	//cout << entities. << endl;

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
				cout << p.state << endl;
				entities.push_back(new Bullet(BulletImage, "Bullet", lvl, p.x, p.y, 16, 16, p.state));
			}//если выстрелили, то появляется пуля.
			if (event.type == Event::KeyPressed)
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
			(*it)->update(time);//вызываем ф-цию update для всех объектов (по сути для тех, кто жив)

			if ((*it)->life == false) // если этот объект мертв, то удаляем его
			{
				it = entities.erase(it);
			}
			else it++;
		}





		///тут идет проверка столкновения врага с игроком

		for (it = entities.begin(); it != entities.end(); it++)
		{
			if ((*it)->name == "Enemy")
			{
				if ((*it)->isShoot == true)
				{
					(*it)->isShoot = false;

					cout << (*it)->dirr << endl;
					entities.push_back(new Bullet(BulletImage, "EnemyBullet", lvl, (*it)->x, (*it)->y, 16, 16, (*it)->dirr));
				}
			}

			if ((*it)->getRect().intersects(p.getRect()))
			{
				if ((*it)->name == "Enemy")
				{
					if ((*it)->dx > 0)
					{
						(*it)->x = p.x - (*it)->w;
						(*it)->dx = 0;

					}
					if ((*it)->dx < 0)
					{
						(*it)->x = p.x + p.w;
						(*it)->dx = 0;
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
			///тут идет уже проверка на контакт врага с пулей, если пуля коснулась врага здоровье его уменьщшается на 50 единиц
			for (list<Entity*>::iterator it2 = entities.begin(); it2 != entities.end(); it2++)
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
					/*for (list<Entity*>::iterator it2 = entities.begin(); it2 != entities.end(); it2++)
					{
						Entity* bullet = *it2;
						if (bullet->name == "EnemyBullet")
						{
							if (bullet->life == true)
							{
								if (bullet->getRect().intersects(p.getRect()))
								{
									bullet->life = false;
									p.health -= 40;
									if (p.health == 0)
									{
										p.life = false;
									}
								}
							}
						}
					}*/
				}
			}



		}

		window.setView(view);
		window.clear();
		lvl.Draw(window);

		//Отрисовка текста
		ostringstream playerHealthString, gameTimeString;
		playerHealthString << p.health;
		text.setString("Armor: " + playerHealthString.str());
		text.setPosition(view.getCenter().x - 500, view.getCenter().y + 500);

		//тут уже идет отрисовка врагов
		for (it = entities.begin(); it != entities.end(); it++)
		{
			window.draw((*it)->sprite);
		}
		window.draw(text);

		window.draw(p.sprite);
		window.display();
	}
}

void exec_v2()
{
	Clock clock;
	float CurrentFrame = 0;
	RenderWindow window(VideoMode(1366, 768), "Project_Tank_Battle");
	window.setFramerateLimit(30);
	view.reset(FloatRect(0, 0, 1920, 1080));
	Level lvl;
	lvl.LoadFromFile("map02.tmx");
	Image heroImage;
	heroImage.loadFromFile("images/tanks.png");

	Image enemyImage;
	enemyImage.loadFromFile("images/tanks.png");

	Image bulletImage;
	bulletImage.loadFromFile("images/bullet.png");
	bulletImage.createMaskFromColor(Color(0, 0, 0));

	Object playerObject = lvl.GetObject("Player");

	Player player(heroImage, "Player", lvl, playerObject.rect.left, playerObject.rect.top, 75, 82, 1);

	list<Enemy*> enemies;
	list<Enemy*>::iterator enemy_it;

	list<Bullet*> bullets;
	list<Bullet*>::iterator bullet_it;

	vector<Object> enemiesObjects = lvl.GetObjects("Enemy");

	for (int i = 0; i < enemiesObjects.size(); i++)
	{
		enemies.push_back(new Enemy(enemyImage, "Enemy", lvl, enemiesObjects[i].rect.left, enemiesObjects[i].rect.top, 71, 80, 1));
	}

	while (window.isOpen())
	{
		Event event;

		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

			if (player.isShoot == true)
			{
				player.isShoot = false;
				bullets.push_back(new Bullet(bulletImage, "Bullet", lvl, player.x, player.y, 16, 16, player.state));
			}

			if (event.type == Event::KeyPressed)
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

		player.update(time);


		for (enemy_it = enemies.begin(); enemy_it != enemies.end();)
		{
			(*enemy_it)->update(time);
			if ((*enemy_it)->life == false)
			{
				enemy_it = enemies.erase(enemy_it);
			}
			else enemy_it++;
		}

		for (bullet_it = bullets.begin(); bullet_it != bullets.end();)
		{
			(*bullet_it)->update(time);
			if ((*bullet_it)->life == false)
			{
				bullet_it = bullets.erase(bullet_it);
			}
			else bullet_it++;
		}

		for (enemy_it = enemies.begin(); enemy_it != enemies.end(); enemy_it++)
		{
			if ((*enemy_it)->isShoot == true)
			{
				(*enemy_it)->isShoot = false;

				bullets.push_back(new Bullet(bulletImage, "EnemyBullet", lvl, (*enemy_it)->x, (*enemy_it)->y, 16, 16, (*enemy_it)->direction));

			}

			if ((*enemy_it)->getRect().intersects(player.getRect()))
			{
				if ((*enemy_it)->dx > 0)
				{
					(*enemy_it)->x = player.x - (*enemy_it)->w;
					(*enemy_it)->dx = 0;

				}
				if ((*enemy_it)->dx < 0)
				{
					(*enemy_it)->x = player.x + player.w;
					(*enemy_it)->dx = 0;
				}
				if (player.dx < 0)
				{
					player.x = (*enemy_it)->x + (*enemy_it)->w;
				}
				if (player.dx > 0)
				{
					player.x = (*enemy_it)->x - player.w;
				}

				if ((*enemy_it)->dy > 0)
				{
					(*enemy_it)->y = player.y - (*enemy_it)->h;
					(*enemy_it)->dy = 0;
				}
				if ((*enemy_it)->dy < 0)
				{
					(*enemy_it)->y = player.y + player.h;
					(*enemy_it)->dy = 0;
				}
				if (player.dy < 0)
				{
					player.y = (*enemy_it)->y + (*enemy_it)->h;
				}
				if (player.dy > 0)
				{
					player.y = (*enemy_it)->y - player.h;
				}
			}

		}
		for (list<Enemy*>::iterator e_it = enemies.begin(); e_it != enemies.end(); e_it++)
		{
			if ((*e_it)->name == "Enemy")
			{
				Enemy* enemy = *e_it;
				for (list<Bullet*>::iterator b_it = bullets.begin(); b_it != bullets.end(); b_it++)
				{
					Bullet* bullet = *b_it;
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
		/*for (list<Bullet*>::iterator b_it2 = entities.begin(); it2 != entities.end(); it2++)
					{
						Entity* bullet = *it2;
						if (bullet->name == "EnemyBullet")
						{
							if (bullet->life == true)
							{
								if (bullet->getRect().intersects(p.getRect()))
								{
									bullet->life = false;
									p.health -= 40;
									if (p.health == 0)
									{
										p.life = false;
									}
								}
							}
						}
					}*/

		window.setView(view);
		window.clear();
		lvl.Draw(window);

		for (enemy_it = enemies.begin(); enemy_it != enemies.end(); enemy_it++)
		{
			window.draw((*enemy_it)->sprite);
		}

		for (bullet_it = bullets.begin(); bullet_it != bullets.end(); bullet_it++)
		{
			window.draw((*bullet_it)->sprite);
		}

		window.draw(player.sprite);
		window.display();

	}





}

int main()
{
	exec_v2();
	//exec();

	return 0;
}

