//This is a function for launch the game
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