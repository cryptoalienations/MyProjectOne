#include <SFML/Graphics.hpp>

using namespace sf;

int game = 0;
float y = 238, CurrentFrame = 0, py = 0, dy = 0;
int characterState = 2;
int bx[2], by[2], bty[2];

int main() {
	RenderWindow window(VideoMode(800, 500), "Running");

	window.setFramerateLimit(60);

	Texture f;
	f.loadFromFile("Images/Materiales for map/Fon1.jpg");
	Sprite fon(f);

	Image plat;
	Texture e;
	Sprite earth[3];
	plat.loadFromFile("Images/Materiales for map/plat2.png");
	plat.createMaskFromColor(Color(255, 255, 255));
	e.loadFromImage(plat);
	for (int i = 0; i < 3; i++) {
		earth[i].setTexture(e);
		earth[i].setScale(1, 0.25);
		earth[i].setPosition(0, 150 + 150 * i);
	}

	Texture p;
	p.loadFromFile("Images/Player/man.png");
	Sprite player(p);
	player.setTextureRect(IntRect(0, 0, 77, 65));

	Image im2;
	Texture b;
	im2.loadFromFile("Images/Materiales for map/ya2.png");
	im2.createMaskFromColor(Color(237, 28, 36));
	b.loadFromImage(im2);
	Sprite block[2];
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

	while (window.isOpen()){

		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();

			if (event.type == Event::KeyPressed) {
				if (event.key.code == Keyboard::Enter) {
					if (game == 0) {
						game = 1;
					}
				}
				if (event.key.code == Keyboard::Up && characterState > 1) {
					characterState--;
				}
				else if (event.key.code == Keyboard::Down && characterState < 3) {
					characterState++;
				}
			}			
		}
		if (game == 1) {

			CurrentFrame += 0.25;
			if (CurrentFrame > 9) CurrentFrame -= 8;
			player.setTextureRect(IntRect(77 * int(CurrentFrame), 0, 77, 65));
			
			if (characterState == 1) {
				y = 88;
			}
			else if (characterState == 3) {
				y = 388;
			}
			else {
				y = 238;
			}

			Vector2f eh[3];
			for (int i = 0; i < 3; i++) {
				earth[i].move(-15.5, 0);
				eh[i] = earth[i].getPosition();

				if (eh[i].x < -800) {
					earth[i].setPosition(0, 150 + 150 * i);
				}
			}

			Vector2f bk[2];
			for (int i = 0; i < 2; i++) {
				block[i].move(-15.5, 0);
				bk[i] = block[i].getPosition();

				if (bk[i].x < -100) {
					bx[i] = rand() % 3 + 1;
					by[i] = rand() % 2 + 1;
					bty[i] = rand() % 3 + 1;
					block[i].setTextureRect(IntRect(100 * (bx[i] - 1), 100 * (by[i] - 1), 100, 100));
					block[i].setPosition(1000 + 500 * i, 50 + 150 * (bty[i] - 1));
				}

				if (player.getGlobalBounds().intersects(block[i].getGlobalBounds())) {
					game = 2;
				}
			}
		}
		else if (game ==2) {
			player.setTextureRect(IntRect(156, 104, 82, 30));
			y = 280;
		}

		window.clear(Color::White);
		window.draw(fon);

		for (int i = 0; i < 3; i++) {
			window.draw(earth[i]);
		}

		player.setPosition(100, y);
		window.draw(player);

		if (game == 0) {
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