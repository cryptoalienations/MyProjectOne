#include <SFML/Graphics.hpp>

using namespace sf;

int main()
{
	srand(time(0));

	RenderWindow window(VideoMode(800, 500), "Running!");

	Texture f;
	f.loadFromFile("Images/Materiales for map/Fon1.jpg");
	Sprite fon(f);

	Image im;
	Texture e;
	im.loadFromFile("Images/Materiales for map/plat2.png");
	im.createMaskFromColor(Color(255, 255, 255));
	e.loadFromImage(im);
	Sprite earth[3];
	for (int i = 0; i < 3; i++) {
		earth[i].setTexture(e);
		earth[i].setScale(1, 0.25);
		earth[i].setPosition(0, 150 + 150 * i);
	}

	Texture m;
	m.loadFromFile("Images/Player/man.png");
	Sprite man(m);
	man.setTextureRect(IntRect(0, 0, 77, 65));
	float y = 238, frame = 0, py = 0, dx = 0, dy = 0, speed = 0.5;
	bool up = false, down = false, anim = true;


	Texture t;
	t.loadFromFile("Images/Materiales for map/plat.png");
	Sprite text(t);
	text.setPosition(300, 50);
	float fx = 0;
	int game = 0;

	Image im2;
	Texture b;
	im2.loadFromFile("Images/Materiales for map/ya2.png");
	im2.createMaskFromColor(Color(237, 28, 36));
	b.loadFromImage(im2);
	Sprite block[2];
	int bx[2], by[2], bty[2];
	for (int i = 0; i < 2; i++) {
		block[i].setTexture(b);
		bx[i] = rand() % 3 + 1;
		by[i] = rand() % 2 + 1;
		bty[i] = rand() % 3 + 1;
		block[i].setTextureRect(IntRect(100 * (bx[i] - 1), 100 * (by[i] - 1), 100, 96));
		block[i].setPosition(1000 + 500 * i, 50 + 155 * (bty[i] - 1));
	}

	Font font;
	font.loadFromFile("Donpoligrafbum-Bold.otf");
	Text text1("", font, 20);
	Text text2("", font, 20);

	while (window.isOpen())
	{

		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed) {
				window.close();
			}

			if (event.type == Event::KeyPressed) {
				if (event.key.code == Keyboard::Enter) {
					if (game == 0) {
						game = 1;
					}
				}
			}
		}
		if (game == 1) {
			if (event.type == Event::KeyPressed) {
				if (event.key.code == Keyboard::Up) {
					dy = -150;

					up = true;
				}
				else if (event.key.code == Keyboard::Down) {
					dy = 150;

					down = true;
				}
			}

		}


		if (game == 0) {
			frame += 0.001;
			if (frame > 4) {
				frame -= 4;
				man.setTextureRect(IntRect(77 * int(frame), 0, 77, 65));
			}
		}
		else if (game == 1) {
			if (py != dy) {
				if (dy > 0) {
					py = speed;
					dy = dy - speed;
				}
				if (dy < 0) {
					py = -speed;
					dy = dy + speed;
				}

				y += py;

				anim = false;
			}

			else {
				up = false;
				down = false;

				anim = true;
			}

			if (y < 55) {
				y = 55;
			}
			else if (y > 355) {
				y = 355;
			}

			if (anim) {
				frame += 0.01;
				if (frame > 6) {
					frame -= 6;
					man.setTextureRect(IntRect(77 * int(frame), 0, 77, 65));
				}

				/*switch (int(frame))
				{
				case 0:
					man.setTextureRect(IntRect(0, 0, 77, 65));
					break;
				case 1:
					man.setTextureRect(IntRect(0, 0, 77, 65));
					break;
				case 2:
					man.setTextureRect(IntRect(0, 0, 77, 65));
					break;
				case 3:
					man.setTextureRect(IntRect(0, 0, 77, 65));
					break;
				case 4:
					man.setTextureRect(IntRect(0, 0, 77, 65));
					break;
				case 5:
					man.setTextureRect(IntRect(0, 0, 77, 65));
					break;
				}*/
			}

			else {
				if (up) {
					man.setTextureRect(IntRect(0, 400, 80, 150));
				}
				if (down) {
					man.setTextureRect(IntRect(0, 400, 100, 150));
				}
			}

			if (event.type == Event::KeyPressed) {
				if (event.key.code == Keyboard::Up) {
					dy = -150;

					up = true;
				}
				else if (event.key.code == Keyboard::Down) {
					dy = 150;

					down = true;
				}
			}

			Vector2f eh[3];
			for (int i = 0; i < 3; i++) {
				earth[i].move(-0.4, 0);

				eh[i] = earth[i].getPosition();
				if (eh[i].x < -800) {
					earth[i].setPosition(0, 150 + 150 * i);
				}
			}

			Vector2f bk[2];
			for (int i = 0; i < 2; i++) {
				block[i].move(-0.4, 0);

				bk[i] = block[i].getPosition();
				if (bk[i].x < -100) {
					bx[i] = rand() % 3 + 1;
					by[i] = rand() % 2 + 1;
					bty[i] = rand() % 3 + 1;
					block[i].setTextureRect(IntRect(100 * (bx[i] - 1), 100 * (by[i] - 1), 100, 100));
					block[i].setPosition(1000 + 500 * i, 50 + 150 * (bty[i] - 1));
				}

				if (man.getGlobalBounds().intersects(block[i].getGlobalBounds())) {
					game = 2;
				}
			}


		}

		else if (game == 2) {
			man.setTextureRect(IntRect(156, 104, 82, 30));
			y = 280;
		}

		window.clear(Color::White);
		window.draw(fon);
		for (int i = 0; i < 3; i++) {
			window.draw(earth[i]);
		}
		man.setPosition(100, y);
		window.draw(man);

		if (game == 0) {
			fx += 0.005;
			if (fx > 5)
				fx -= 5;
			if (fx < 3)
				text1.setString(L"Press ENTER to start");
			text1.setPosition(190, 250);
			window.draw(text1);
		}

		for (int i = 0; i < 2; i++) {
			window.draw(block[i]);
		}

		if (game == 2) {
			text2.setString(L"Game Over");
			text2.setPosition(300, 250);
			window.draw(text2);

		}

		window.display();
	}

	return 0;
}

