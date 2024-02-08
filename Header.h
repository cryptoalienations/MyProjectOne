#include <SFML/Graphics.hpp>
#include <conio.h>
#include <intrin.h>

using namespace sf;

float offsetX = 0, offsetY = 0; //������

////�����////
const int H = 10; //���������� ������ �� ������
const int W = 50; //���-�� ������ �� ������

const int ts = 50; //������ ��������

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

/////��������/////
class Player {
public:
	float dx, dy;    //��������
	FloatRect rect;  //������ 4�� ��������� ��������� �/� � �.�. ������ � 1 (rect)
	bool onGround, rig;   //��������� ������� �� ����� �� ��������� ��� ���
	Sprite sprite;   //���� ��������� ��������
	float curFrame;  //������� ���� ��� ��������
	bool life;  //����� ���
	int l;      //���������� �����

	Player(Texture& image) {
		sprite.setTexture(image);
		rect = FloatRect(7 * ts, 8 * ts, 77, 65); //������������ ������ �� �����
		sprite.setTextureRect(IntRect(77, 0, 77, 65));
		//sprite.setScale(0.25, 0.25); //���������� ���������

		dx = dy = 0;
		curFrame = 1;
		rig = true;
		life = true;
		l = 3;  //���-�� �����
	}

	void update(float time) {
		rect.left += dx * time; //���������� �
		Collision(0); //��� X


		if (!onGround)
			dy = dy + 0.0005 * time; //���� �� �� �� ����� �� ������ � ���������� � ���������� � �������� (0.0005 ���������)

		rect.top += dy * time; //���������� Y

		onGround = false;

		Collision(1); //��� Y
		if (life) {

			if (l == 0)
				life = false;

			curFrame += 0.01 * time;
			if (curFrame > 9) //���-�� ������ ���������
				curFrame -= 8; //������� ���������

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

	void Collision(int dir) {   //�������������� � ������(������������)
		for (int i = rect.top / ts; i < (rect.top + rect.height) / ts; i++)
			for (int j = rect.left / ts; j < (rect.left + rect.width) / ts; j++) {
				if (TileMap[i][j] == 'A') { //��� ������ A
					if (dx > 0 && dir == 0) //��� �
						rect.left = j * ts - rect.width;
					if (dx < 0 && dir == 0)
						rect.left = j * ts + ts;
					if (dy > 0 && dir == 1) { //��� Y
						rect.top = i * ts - rect.height;
						dy = 0; //���� ����� �������� ������
						onGround = true; //�������� �������, ���� ��� ������ �� ������
					}
					if (dy < 0 && dir == 1) {
						rect.top = i * ts + ts;
						dy = 0;

					}
				}

				if (TileMap[i][j] == 'o') //���� ����� ����� ����������� � �������� "�" ��� ��������
					TileMap[i][j] = ' ';
				if (TileMap[i][j] == 'c') { //���� ����� ����� ����������� � �������� "c" ��� ��������
					TileMap[i][j] = ' ';

					l++;
				}
			}
	}

};

////�������////
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

////���////
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

//�������� ���� ����//
int main() {
	RenderWindow window(VideoMode(1000, 500), "Platformer");

	float Frame = 0;

	Texture Picture;  //����������� ������
	Picture.loadFromFile("Images/Player/man1.png");
	Player p(Picture);
	int frame = 0;

	Texture t1; //����������� ����
	t1.loadFromFile("Images/Shoots/pula.png");
	Pula pu(t1);

	Texture t2;  //����������� ���������� �����
	t2.loadFromFile("Images/Materiales for map/plat.png");
	Sprite plat(t2);

	Texture t3;  //����������� ����
	t3.loadFromFile("Images/Bot/enemy.png");
	Enemy enemy;
	enemy.set(t3, 19 * ts, 8 * ts - 20);

	Texture t4;  //����������� ����� ������
	t4.loadFromFile("Images/Player/life.png");
	Sprite life[8]; //�������� ������� ������ ��������
	for (int i = 0; i < 8; i++) {
		life[i].setTexture(t4);
		life[i].setScale(1.5, 1.5); //������ �����������
		life[i].setPosition(5 + 50 * i, 5); //������� ����������� (� ����� ������� ����)
	}

	Clock clock;  //��� �������� �� �������

	while (window.isOpen())
	{
		float time = clock.getElapsedTime().asMicroseconds();
		clock.restart();
		time = time / 800; //����������� �������� ����

		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();
		}

		if (!_kbhit()) //������� ���� ������� �������� � �������� �� ���������
			if (p.dy == 0) {
				if (p.rig)
					p.sprite.setTextureRect(IntRect(0, 0, 77, 65));  //������
				else if (!p.rig)
					p.sprite.setTextureRect(IntRect(77, 0, -77, 65)); //�����������
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

						pu.rect.left = p.rect.left + 67;  //� ���� = � ������ + ����� ������
						pu.rect.top = p.rect.top + 20;   //Y ���� = Y ������ + �������� ������
					}
					else {
						pu.go = 2;

						pu.rect.left = p.rect.left - 12; //��������� � ���� ������ ������
						pu.rect.top = p.rect.top + 18;  // ��� ���� ����� ������ +
					}
				}
		}
		if (pu.go == 0) { //�������, ��� ���� �������� ���, �� ���� ��������� � �������
			pu.rect.left = p.rect.left;
			pu.rect.top = p.rect.top;
		}

		else if (pu.go == 1)
			pu.dx = 1; //���� ����� 1 �� ����� ���� ������
		else if (pu.go == 2)
			pu.dx = -1; // ���� ����� 2 �� ����� �����


		if (frame != 0)
			if (p.dy == 0) {
				if (p.rig)
					p.sprite.setTextureRect(IntRect(68, 80, 69, 61));
				else if (!p.rig)
					p.sprite.setTextureRect(IntRect(68 + 69, 80, -69, 61));

				frame--;

			}


		//�������������� � �����
		if (enemy.life) {
			if (p.rect.intersects(enemy.rect)) { //����� ����� ������������ � ����� �� �������� ����
				p.l--;

				if (p.l > 0) {
					if (p.rig)
						p.rect.left -= 100;
					else
						p.rect.left += 100;
				}
			}

			//�������, ��� ���� ����������� � �����������
			if (enemy.rect.left<pu.rect.left && enemy.rect.left + 10 > pu.rect.left &&
				enemy.rect.top < pu.rect.top && enemy.rect.top + 50 > pu.rect.top && pu.go != 0) {
				enemy.life = false;
				pu.go = 0;
			}
		}

		if (p.rect.left > 500 && p.rect.left < 2000) //�������, ��� � ����� � ����� ������ �������, � ����������� ����� ����� (p.rect.left < 2000 �� ��� ������� ���� �������/����������� ������)
			offsetX = p.rect.left - 500;  //�������� ������ � ������, ���� ���� �� ������ ������ ����� �� 2
		if (p.rect.top > 500)  //������� ��� � ����� ����� ������ �������
			offsetY = p.rect.top - 250;  //�������� ������ � ������, ���� ���� �� ������ ������ ����� �� 2

		p.update(time);
		pu.update(time);
		enemy.update(time);
		window.clear(Color::White);

		for (int i = 0; i < H; i++) //���� �����
			for (int j = 0; j < W; j++) {
				if (TileMap[i][j] == 'A')
					plat.setTextureRect(IntRect(0, 0, ts, ts));  //���������� � png � ������ ������ ��� �������� �����
				if (TileMap[i][j] == 'o')
					plat.setTextureRect(IntRect(ts, 0, ts, ts)); //���������� � png � ������ ������ ��� HP
				if (TileMap[i][j] == 'c')
					plat.setTextureRect(IntRect(ts * 2, 0, ts, ts));
				if (TileMap[i][j] == ' ')
					continue;

				plat.setPosition(j * ts - offsetX, i * ts - offsetY);
				window.draw(plat);
			}

		window.draw(p.sprite); //����������� ����������� ������ � ����
		window.draw(enemy.sprite);  //���
		if (pu.go != 0) //����
			window.draw(pu.sprite); //����
		for (int i = 0; i < p.l; i++) //�����
			window.draw(life[i]);  //�����
		window.display();
	}

	return 0;
}


