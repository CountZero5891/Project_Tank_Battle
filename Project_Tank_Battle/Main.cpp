#include <SFML/Graphics.hpp>
#include <string>
#include <sstream>
#include <iostream>
#include "level.h"
#include <vector>
#include <list>
#include "Entity.h"
#include "Player.h"
#include "Enemy.h"
#include "Bullet.h"

using namespace sf;
using namespace std;

void CollisionBetweenObject()
{
	//Заготовка для функции которая проверяет взаимодействия между объектами
}

void update_bullets(list<Bullet*> bullets, list<Bullet*>::iterator bullet_it, float &time)
{
	for (bullet_it = bullets.begin(); bullet_it != bullets.end();)
	{
		(*bullet_it)->update(time);
		if ((*bullet_it)->life == false)
		{
			bullet_it = bullets.erase(bullet_it);
		}
		else bullet_it++;
	}
}

void update_enemies(list<Enemy*> enemies, list<Enemy*>::iterator enemy_it, float& time)
{
	for (enemy_it = enemies.begin(); enemy_it != enemies.end();)
	{
		(*enemy_it)->update(time);
		if ((*enemy_it)->life == false)
		{
			enemy_it = enemies.erase(enemy_it);
		}
		else enemy_it++;
	}
}


void exec_v2()
{
	Clock clock;
	float CurrentFrame = 0;
	RenderWindow window(VideoMode(1366, 768), "Project_Tank_Battle");
	window.setFramerateLimit(30);
	view.reset(FloatRect(50, 50, 1920, 1080));
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


		/*for (enemy_it = enemies.begin(); enemy_it != enemies.end();)
		{
			(*enemy_it)->update(time);
			if ((*enemy_it)->life == false)
			{
				enemy_it = enemies.erase(enemy_it);
			}
			else enemy_it++;
		}*/

		/*for (bullet_it = bullets.begin(); bullet_it != bullets.end();)
		{
			(*bullet_it)->update(time);
			if ((*bullet_it)->life == false)
			{
				bullet_it = bullets.erase(bullet_it);
			}
			else bullet_it++;
		}*/
		update_enemies(enemies, enemy_it, time);

		update_bullets(bullets, bullet_it, time);

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

