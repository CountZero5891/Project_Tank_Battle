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

//void update_bullets(list<Bullet*> &bullets, list<Bullet*>::iterator bullet_it, float &time)
//{
//	for (bullet_it = bullets.begin(); bullet_it != bullets.end();)
//	{
//		(*bullet_it)->update(time);
//		if ((*bullet_it)->life == false)
//		{
//			bullet_it = bullets.erase(bullet_it);
//		}
//		else bullet_it++;
//	}
//}
//
void update_enemies(list<Entity*> &enemies, list<Entity*>::iterator enemy_it, float& time)
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

void entity_update(list<Entity*>& entities, list<Entity*>::iterator entity_it, float& time, Player &player, Level &lvl/*, AnimationManager &anim*/ )
{
	for (entity_it = entities.begin(); entity_it != entities.end();)
	{
		(*entity_it)->update(time);
		if ((*entity_it)->life == false)
		{
			entity_it = entities.erase(entity_it);
		}
		else entity_it++;
	}

	for (entity_it = entities.begin(); entity_it != entities.end(); entity_it++)
	{
		if ((*entity_it)->name == "Enemy")
		{
			if ((*entity_it)->isShoot == true)
			{
				(*entity_it)->isShoot = false;

				entities.push_back(new Bullet((*entity_it)->anim, "EnemyBullet", lvl, (*entity_it)->x, (*entity_it)->y, 16, 16, 1/*(*entity_it)->direction*/));

			}
		}
		

		if ((*entity_it)->getRect().intersects(player.getRect()))
		{

			if ((*entity_it)->name == "Enemy")
			{
				if ((*entity_it)->dx > 0)
				{
					(*entity_it)->x = player.x - (*entity_it)->w;
					(*entity_it)->dx = 0;

				}
				if ((*entity_it)->dx < 0)
				{
					(*entity_it)->x = player.x + player.w;
					(*entity_it)->dx = 0;
				}
				if (player.dx < 0)
				{
					player.x = (*entity_it)->x + (*entity_it)->w;
				}
				if (player.dx > 0)
				{
					player.x = (*entity_it)->x - player.w;
				}

				if ((*entity_it)->dy > 0)
				{
					(*entity_it)->y = player.y - (*entity_it)->h;
					(*entity_it)->dy = 0;
				}
				if ((*entity_it)->dy < 0)
				{
					(*entity_it)->y = player.y + player.h;
					(*entity_it)->dy = 0;
				}
				if (player.dy < 0)
				{
					player.y = (*entity_it)->y + (*entity_it)->h;
				}
				if (player.dy > 0)
				{
					player.y = (*entity_it)->y - player.h;
				}
			}

			
		}
	}
	for (list<Entity*>::iterator entity_it = entities.begin(); entity_it != entities.end(); entity_it++)
	{
		if ((*entity_it)->name == "Enemy")
		{
			Entity* enemy = *entity_it;
			for (list<Entity*>::iterator b_it = entities.begin(); b_it != entities.end(); b_it++)
			{
				Entity* bullet = *b_it;
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

	for (list<Entity*>::iterator b_it2 = entities.begin(); b_it2 != entities.end(); b_it2++)
	{
		Entity* bullet = *b_it2;
		if (bullet->name == "EnemyBullet")
		{
			if (bullet->life == true)
			{
				if (bullet->getRect().intersects(player.getRect()))
				{
					bullet->life = false;
					player.health -= 40;
					if (player.health == 0)
					{
						player.life = false;
					}
				}
			}
		}
	}


}

//
//void another_check(list<Enemy*>  &enemies, list<Enemy*>::iterator enemy_it, list<Bullet*> &bullets, Image &bulletImage, Level &lvl, Player &player)
//{
//	for (enemy_it = enemies.begin(); enemy_it != enemies.end(); enemy_it++)
//	{
//		if ((*enemy_it)->isShoot == true)
//		{
//			(*enemy_it)->isShoot = false;
//
//			bullets.push_back(new Bullet(bulletImage, "EnemyBullet", lvl, (*enemy_it)->x, (*enemy_it)->y, 16, 16, (*enemy_it)->direction));
//
//		}
//
//		if ((*enemy_it)->getRect().intersects(player.getRect()))
//		{
//			if ((*enemy_it)->dx > 0)
//			{
//				(*enemy_it)->x = player.x - (*enemy_it)->w;
//				(*enemy_it)->dx = 0;
//
//			}
//			if ((*enemy_it)->dx < 0)
//			{
//				(*enemy_it)->x = player.x + player.w;
//				(*enemy_it)->dx = 0;
//			}
//			if (player.dx < 0)
//			{
//				player.x = (*enemy_it)->x + (*enemy_it)->w;
//			}
//			if (player.dx > 0)
//			{
//				player.x = (*enemy_it)->x - player.w;
//			}
//
//			if ((*enemy_it)->dy > 0)
//			{
//				(*enemy_it)->y = player.y - (*enemy_it)->h;
//				(*enemy_it)->dy = 0;
//			}
//			if ((*enemy_it)->dy < 0)
//			{
//				(*enemy_it)->y = player.y + player.h;
//				(*enemy_it)->dy = 0;
//			}
//			if (player.dy < 0)
//			{
//				player.y = (*enemy_it)->y + (*enemy_it)->h;
//			}
//			if (player.dy > 0)
//			{
//				player.y = (*enemy_it)->y - player.h;
//			}
//		}
//	}
//}

//void check_collision_bullet(list<Enemy*> &enemies, list<Enemy*>::iterator it, list<Bullet*> &bullets, Player &player)
//{
//	for (list<Enemy*>::iterator e_it = enemies.begin(); e_it != enemies.end(); e_it++)
//	{
//		if ((*e_it)->name == "Enemy")
//		{
//			Enemy* enemy = *e_it;
//			for (list<Bullet*>::iterator b_it = bullets.begin(); b_it != bullets.end(); b_it++)
//			{
//				Bullet* bullet = *b_it;
//				if (bullet->name == "Bullet")
//				{
//					if (bullet->life == true)
//					{
//						if (bullet->getRect().intersects(enemy->getRect()))
//						{
//							bullet->life = false;
//							enemy->health -= 50;
//							if (enemy->health == 0)
//							{
//								enemy->life = false;
//							}
//						}
//					}
//				}
//			}
//		}
//	}
//
//	for (list<Bullet*>::iterator b_it2 = bullets.begin(); b_it2 != bullets.end(); b_it2++)
//	{
//		Bullet* bullet = *b_it2;
//		if (bullet->name == "EnemyBullet")
//		{
//			if (bullet->life == true)
//			{
//				if (bullet->getRect().intersects(player.getRect()))
//				{
//					bullet->life = false;
//					player.health -= 40;
//					if (player.health == 0)
//					{
//						player.life = false;
//					}
//				}
//			}
//		}
//	}
//}


//Test animate in class


void exec_v3()
{
	//Fonts initialize
	Font font;
	font.loadFromFile("fonts/pcsenior.ttf");
	Text text("", font, 20);
	text.setFillColor(Color::Green);
	//	//	//	//	//	//	//	//	//	//	//


	Clock clock;
	float CurrentFrame = 0;
	RenderWindow window(VideoMode(1366, 768), "Project_Tank_Battle");
	window.setFramerateLimit(30);
	view.reset(FloatRect(50, 50, 1920, 1080));
	Level lvl;
	lvl.LoadFromFile("map02.tmx");
	
	Object playerObject = lvl.GetObject("Player");

	Image heroImage;
	heroImage.loadFromFile("images/tanks.png");
	Image bulletImage;
	bulletImage.loadFromFile("images/bullet.png");
	bulletImage.createMaskFromColor(Color(0, 0, 0));

	Image enemyImage;
	enemyImage.loadFromFile("images/tanks.png");

	//Initializing player//
	Texture player_texture;
	Texture player_bullet;

	player_texture.loadFromFile("images/tanks.png");
	player_bullet.loadFromFile("images/tanks.png");
	AnimationManager playerAnim;//
	AnimationManager plrbltAnim;//player bullet anim
	plrbltAnim.create("bullet_move", player_bullet, 360, 200, 19, 19, 1, 0);
	playerAnim.create("up", player_texture, 0, 85, 75, 82, 8, 0.005, 90, 0);
	playerAnim.create("down", player_texture, 0, 85, 75, 82, 8, 0.005, 90, 180);
	playerAnim.create("left", player_texture, 0, 85, 75, 82, 8, 0.005, 90, -90);
	playerAnim.create("right", player_texture, 0, 85, 75, 82, 8, 0.005, 90, 90);
	Player player(playerAnim, "Player", lvl, playerObject.rect.left, playerObject.rect.top, 75, 82, 2);
	////////////////////////////////////////////////////////////////////////////
	

	//Initialize enemies//
	Texture enemy_texture;
	enemy_texture.loadFromFile("images/tanks.png");
	AnimationManager enemyAnim;
	enemyAnim.create("enemy_up", enemy_texture, 0, 1, 75, 82, 8, 0.005, 90, 0);
	enemyAnim.create("enemy_down", enemy_texture, 0, 1, 75, 82, 8, 0.005, 90, 180);
	enemyAnim.create("enemy_left", enemy_texture, 0, 1, 75, 82, 8, 0.005, 90, -90);
	enemyAnim.create("enemy_right", enemy_texture, 0, 1, 75, 82, 8, 0.005, 90, 90);
	
	vector<Object> enemiesObjects = lvl.GetObjects("Enemy");
	list<Entity*> entities;
	list<Entity*>::iterator enityy_it;
	list<Entity*>::iterator entity_it2;
	for (int i = 0; i < enemiesObjects.size(); i++)
	{
		entities.push_back(new Enemy(enemyAnim, "Enemy", lvl, enemiesObjects[i].rect.left, enemiesObjects[i].rect.top, 71, 80, 1));
	}

	/////////////////////////////

	

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
				entities.push_back(new Bullet(plrbltAnim, "Bullet", lvl, (player.x + player.w / 3), (player.y + player.h / 3), 19, 19, player.STATE));
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
		//cout << player.STATE << endl;
		time = time / 800;

		player.update(time);
		entity_update(entities, enityy_it, time, player, lvl);


		window.setView(view);
		window.clear();
		lvl.Draw(window);


		//window.draw(player.sprite);
		player.draw(window);
		plrbltAnim.draw(window, 150, 150);
		for (enityy_it = entities.begin(); enityy_it != entities.end(); enityy_it++)
		{
			(*enityy_it)->draw(window);
		}
		/*for (enemy_it = enemies.begin(); enemy_it != enemies.end(); enemy_it++)
		{
			window.draw((*enemy_it)->sprite);
		}*/
		ostringstream playerHealthString, playerXString, playerYString;
		playerHealthString << player.health;
		playerYString << player.y;
		playerXString << player.x;

		text.setString("Health: " + playerHealthString.str() + " X: " + playerXString.str() + " Y: " + playerYString.str());


		text.setPosition(view.getCenter().x - 700, view.getCenter().y - 500);
		window.draw(text);

		window.display();
	}
}

int main()
{
	exec_v3();
	return 0;
}

/*
void exec_v2()
{
	//Fonts initialize
	Font font;
	font.loadFromFile("fonts/pcsenior.ttf");
	Text text("", font, 20);
	 text.setFillColor(Color::Green);
	//	//	//	//	//	//	//	//	//	//	//


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

	int Dir = 0;
	//Dir = rand()%

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
				bullets.push_back(new Bullet(bulletImage, "Bullet", lvl, player.x, player.y, 16, 16, player.STATE));

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

		update_enemies(enemies, enemy_it, time);

		update_bullets(bullets, bullet_it, time);

		another_check(enemies, enemy_it, bullets, bulletImage, lvl, player);

		check_collision_bullet(enemies, enemy_it, bullets, player);



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

		ostringstream playerHealthString;
		playerHealthString << player.health;
		text.setString("Health: " + playerHealthString.str());
		text.setPosition(view.getCenter().x-700, view.getCenter().y-500);
		window.draw(text);

		window.display();

	}



}*/