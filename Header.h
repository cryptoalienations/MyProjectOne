#include <SFML/Graphics.hpp>
#include <conio.h>
#include <intrin.h>

using namespace sf;

float offsetX = 0, offsetY = 0; //камера

////Карта////
const int H = 10; //Количество блоков по высоте
const int W = 50; //Кол-во блоков по длинне

const int ts = 50; //Размер плитками

String TileMap[H] = {
"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA",
"A                                                A",
"A        occo                                    A",
"A                                      A         A",
"A       AAAAAA                         AAAA      A",
"A                                      A         A",
"AAAAA                           AAA    A      AAAA",
"A                     ocooco           A         A",
"A              AAAA                    AAAA      A",
"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA",
};

/////Персонаж/////
class Player {
public:
	float dx, dy;    //скорость
	FloatRect rect;  //вместо 4ёх перменных координат ш/в и т.д. храним в 1 (rect)
	bool onGround, rig;   //пременная говорит на земле мы находимся или нет
	Sprite sprite;   //сюда загружаем картинку
	float curFrame;  //текущий кадр для анимации
	bool life;  //игрок жив
	int l;      //коэффицент жизни

	Player(Texture& image) {
		sprite.setTexture(image);
		rect = FloatRect(7 * ts, 8 * ts, 77, 65); //расположение игрока на карте
		sprite.setTextureRect(IntRect(77, 0, 77, 65));
		//sprite.setScale(0.25, 0.25); //Уменьшение персонажа

		dx = dy = 0;
		curFrame = 1;
		rig = true;
		life = true;
		l = 3;  //кол-во жизни
	}

	void update(float time) {
		rect.left += dx * time; //координата Х
		Collision(0); //Для X


		if (!onGround)
			dy = dy + 0.0005 * time; //если мы не на земле то падает с ускорением и прибавляем к скорости (0.0005 ускорение)

		rect.top += dy * time; //координата Y

		onGround = false;

		Collision(1); //Для Y
		if (life) {

			if (l == 0)
				life = false;

			curFrame += 0.01 * time;
			if (curFrame > 9) //кол-во кадров персонажа
				curFrame -= 8; //сколько проиграть

			if (dx > 0) {
				if (dy == 0)
					sprite.setTextureRect(IntRect(77 * int(curFrame), 0, 77, 65));
				else
					sprite.setTextureRect(IntRect(0, 81, 45, 65));
				rig = true;
			}
			if (dx < 0) {
				if (dy == 0)
					sprite.setTextureRect(IntRect(77 * int(curFrame) + 77, 0, -77, 65));
				else
					sprite.setTextureRect(IntRect(45, 81, -45, 65));
				rig = false;
			}
		}

		else {
			if (rig)
				sprite.setTextureRect(IntRect(156, 104, 82, 30));
			else
				sprite.setTextureRect(IntRect(238, 104, -82, 30));
		}

		sprite.setPosition(rect.left - offsetX, rect.top - offsetY);

		dx = 0;

	}

	void Collision(int dir) {   //Взаимодействие с картой(столкновения)
		for (int i = rect.top / ts; i < (rect.top + rect.height) / ts; i++)
			for (int j = rect.left / ts; j < (rect.left + rect.width) / ts; j++) {
				if (TileMap[i][j] == 'A') { //Для строки A
					if (dx > 0 && dir == 0) //Для Х
						rect.left = j * ts - rect.width;
					if (dx < 0 && dir == 0)
						rect.left = j * ts + ts;
					if (dy > 0 && dir == 1) { //Для Y
						rect.top = i * ts - rect.height;
						dy = 0; //чтоб игрок перестал падать
						onGround = true; //Является правдой, чтоб мог ходить по блокам
					}
					if (dy < 0 && dir == 1) {
						rect.top = i * ts + ts;
						dy = 0;

					}
				}

				if (TileMap[i][j] == 'o') //Чтоб когда игрок сталкивался с кубиками "о" они исчезали
					TileMap[i][j] = ' ';
				if (TileMap[i][j] == 'c') { //Чтоб когда игрок сталкивался с кубиками "c" они исчезали
					TileMap[i][j] = ' ';

					l++;
				}
			}
	}

};

////выстрел////
class Pula {
public:
	float dx, dy;
	FloatRect rect;
	int go;
	Sprite sprite;

	Pula(Texture& image) {
		sprite.setTexture(image);
		dx = dy = 0;
		go = 0;
	}

	void update(float time) {
		rect.left += dx * time;
		Collision(0);

		rect.top += dy * time;
		Collision(1);

		sprite.setPosition(rect.left - offsetX, rect.top - offsetY);
	}

	void Collision(int dir) {
		for (int i = rect.top / ts; i < (rect.top + rect.height) / ts; i++)
			for (int j = rect.left / ts; j < (rect.left + rect.width) / ts; j++)
				if (TileMap[i][j] == 'A')
					if ((dx > 0 && dir == 0) || (dx < 0 && dir == 0))
						go = 0;
	}
};

////Бот////
class Enemy {
public:
	float dx, dy;
	FloatRect rect;
	Sprite sprite;
	float curFrame;
	bool life;

	void set(Texture& image, int x, int y) {
		sprite.setTexture(image);
		sprite.setScale(0.35, 0.35);
		rect = FloatRect(x, y, ts, ts);

		dx = 0.1;
		curFrame = 0;
		life = true;
	}

	void update(float time) {
		rect.left += dx * time;

		Collision();

		if (life) {
			curFrame += time * 0.005;
			if (curFrame > 5)
				curFrame -= 5;

			if (dx > 0)
				sprite.setTextureRect(IntRect(165 * int(curFrame), 0, 165, 204));
			else if (dx < 0)
				sprite.setTextureRect(IntRect(165 * int(curFrame) + 165, 0, -165, 204));
		}
		else
			sprite.setTextureRect(IntRect(0, 0, 0, 0));

		sprite.setPosition(rect.left - offsetX, rect.top - offsetY);
	}

	void Collision() {
		for (int i = rect.top / ts; i < (rect.top + rect.height) / ts; i++)
			for (int j = rect.left / ts; j < (rect.left + rect.width) / ts; j++)
				if (TileMap[i][j] == 'A') {
					if (dx > 0) {
						rect.left = j * ts - rect.width;
						dx *= -1;
					}
					else if (dx < 0) {
						rect.left = j * ts + ts;
						dx *= -1;
					}
				}
	}
};

//Основной цикл игры//
int main() {
	RenderWindow window(VideoMode(1000, 500), "Platformer");

	float Frame = 0;

	Texture Picture;  //Изображение игрока
	Picture.loadFromFile("Images/Player/man1.png");
	Player p(Picture);
	int frame = 0;

	Texture t1; //Изображение пули
	t1.loadFromFile("Images/Shoots/pula.png");
	Pula pu(t1);

	Texture t2;  //Изображение материалов карты
	t2.loadFromFile("Images/Materiales for map/plat.png");
	Sprite plat(t2);

	Texture t3;  //Изображение бота
	t3.loadFromFile("Images/Bot/enemy.png");
	Enemy enemy;
	enemy.set(t3, 19 * ts, 8 * ts - 20);

	Texture t4;  //Изображение жизни игрока
	t4.loadFromFile("Images/Player/life.png");
	Sprite life[8]; //отвечает сколько жизней максимум
	for (int i = 0; i < 8; i++) {
		life[i].setTexture(t4);
		life[i].setScale(1.5, 1.5); //размер изображения
		life[i].setPosition(5 + 50 * i, 5); //позиция изображения (в левом верхнем углу)
	}

	Clock clock;  //для привязки по времени

	while (window.isOpen())
	{
		float time = clock.getElapsedTime().asMicroseconds();
		clock.restart();
		time = time / 800; //регулировка скорости игры

		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();
		}

		if (!_kbhit()) //условие если клавиша отпущена и персонаж не двигается
			if (p.dy == 0) {
				if (p.rig)
					p.sprite.setTextureRect(IntRect(0, 0, 77, 65));  //вправо
				else if (!p.rig)
					p.sprite.setTextureRect(IntRect(77, 0, -77, 65)); //отзеркалено
			}

		if (p.life) {
			if (Keyboard::isKeyPressed(Keyboard::A))
				p.dx = -0.2;
			if (Keyboard::isKeyPressed(Keyboard::D))
				p.dx = 0.2;

			if (Keyboard::isKeyPressed(Keyboard::W))
				if (p.onGround) {
					p.dy = -0.4;

					p.onGround = false;

					if (p.rig)
						p.sprite.setTextureRect(IntRect(0, 81, 45, 65));
					else if (!p.rig)
						p.sprite.setTextureRect(IntRect(45, 81, -45, 65));
				}

			if (Keyboard::isKeyPressed(Keyboard::Space))
				if (p.onGround && pu.go == 0) {
					frame = 100;

					if (p.rig) {
						pu.go = 1;

						pu.rect.left = p.rect.left + 67;  //Х пули = Х игрока + длина игрока
						pu.rect.top = p.rect.top + 20;   //Y пули = Y игрока + половина ширины
					}
					else {
						pu.go = 2;

						pu.rect.left = p.rect.left - 12; //находится у ркуи самого игрока
						pu.rect.top = p.rect.top + 18;  // тут тоже самое только +
					}
				}
		}
		if (pu.go == 0) { //условие, что если выстрела нет, то пуля находится с игроком
			pu.rect.left = p.rect.left;
			pu.rect.top = p.rect.top;
		}

		else if (pu.go == 1)
			pu.dx = 1; //если равно 1 то летит пуля вправо
		else if (pu.go == 2)
			pu.dx = -1; // если равно 2 то летит влево


		if (frame != 0)
			if (p.dy == 0) {
				if (p.rig)
					p.sprite.setTextureRect(IntRect(68, 80, 69, 61));
				else if (!p.rig)
					p.sprite.setTextureRect(IntRect(68 + 69, 80, -69, 61));

				frame--;

			}


		//Взаимодействие с ботом
		if (enemy.life) {
			if (p.rect.intersects(enemy.rect)) { //когда игрок сталкивается с ботом он получает урон
				p.l--;

				if (p.l > 0) {
					if (p.rig)
						p.rect.left -= 100;
					else
						p.rect.left += 100;
				}
			}

			//условие, что пуля столкнулась с противником
			if (enemy.rect.left<pu.rect.left && enemy.rect.left + 10 > pu.rect.left &&
				enemy.rect.top < pu.rect.top && enemy.rect.top + 50 > pu.rect.top && pu.go != 0) {
				enemy.life = false;
				pu.go = 0;
			}
		}

		if (p.rect.left > 500 && p.rect.left < 2000) //условие, что б слева и права небыло пустоты, а отображался конец карты (p.rect.left < 2000 На это условие надо считать/прикидывать самому)
			offsetX = p.rect.left - 500;  //привязка камеры к игроку, чтоб была по центру размер делим на 2
		if (p.rect.top > 500)  //условие что б снизу карты небыло пустоты
			offsetY = p.rect.top - 250;  //привязка камеры к игроку, чтоб была по центру размер делим на 2

		p.update(time);
		pu.update(time);
		enemy.update(time);
		window.clear(Color::White);

		for (int i = 0; i < H; i++) //цикл карты
			for (int j = 0; j < W; j++) {
				if (TileMap[i][j] == 'A')
					plat.setTextureRect(IntRect(0, 0, ts, ts));  //Координаты в png и размер кубика для основной карты
				if (TileMap[i][j] == 'o')
					plat.setTextureRect(IntRect(ts, 0, ts, ts)); //Координаты в png и размер кубика для HP
				if (TileMap[i][j] == 'c')
					plat.setTextureRect(IntRect(ts * 2, 0, ts, ts));
				if (TileMap[i][j] == ' ')
					continue;

				plat.setPosition(j * ts - offsetX, i * ts - offsetY);
				window.draw(plat);
			}

		window.draw(p.sprite); //Отоброжение изображения игрока в игре
		window.draw(enemy.sprite);  //бот
		if (pu.go != 0) //Пуля
			window.draw(pu.sprite); //Пуля
		for (int i = 0; i < p.l; i++) //жизни
			window.draw(life[i]);  //жизни
		window.display();
	}

	return 0;
}


