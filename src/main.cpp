#include <SFML/Graphics.hpp>
#include "deck.cpp"
#include "game.cpp"
#include "toast.cpp"
#include "cardEntity.cpp"
#include "cardManager.cpp"
#include "endscreen.cpp"

using namespace sf;

int main()
{
	RenderWindow window(VideoMode(800, 600), "Black Jack");
	window.setPosition(Vector2i(200,200));
	window.setFramerateLimit(60);
	
	// Init textures and Sprites
	Texture bgtext;
	bgtext.loadFromFile("assets/background.png");
	Texture cardtext;
	cardtext.loadFromFile("assets/cards.png");
	Texture bettingtext;
	bettingtext.loadFromFile("assets/betdialog.png");
	Texture uitext;
	uitext.loadFromFile("assets/overlay.png");
	Sprite background;
	background.setTexture(bgtext);
	Sprite topbar;
	topbar.setTexture(uitext);
	topbar.setTextureRect(IntRect(0, 0, 800, 50));
    topbar.setPosition(Vector2f(0, 0));
	Sprite bottombar;
	bottombar.setTexture(uitext);
	bottombar.setTextureRect(IntRect(0, 50, 800, 50));
    bottombar.setPosition(Vector2f(0, 550));
	Sprite betdialog;
	betdialog.setTexture(bettingtext);
	betdialog.setPosition(100, 100);
	Font font;
	font.loadFromFile("assets/Beholder.ttf");
	Text betText;
	Text bustText;
	Text splitText;
	Text moneyText;
	Text buttonText;
	Text toastText;
	betText.setFont(font);
	betText.setCharacterSize(50);
	betText.setPosition(10, -8);
	splitText.setFont(font);
	splitText.setCharacterSize(50);
	bustText.setFont(font);
	bustText.setCharacterSize(50);
	moneyText.setFont(font);
	moneyText.setCharacterSize(50);
	moneyText.setPosition(500, -8);
	buttonText.setFont(font);
	buttonText.setCharacterSize(40);
	buttonText.setColor(Color::Black);
	toastText.setFont(font);
	toastText.setCharacterSize(40);

	Game game;
	Endscreen endscreen;

	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed) {
				window.close();
			}
			else if (event.type == Event::MouseButtonPressed) {
				if (event.mouseButton.button == sf::Mouse::Left)
    			{
					if (game.isBettingActive()) {
						if (event.mouseButton.x > 100 && event.mouseButton.x < 700 && event.mouseButton.y > 100 && event.mouseButton.y < 500) {
							int col = (event.mouseButton.x - 100)/200;
							int row = (event.mouseButton.y - 100)/200;
							game.setBet(row, col);
						}
					}
					else if (event.mouseButton.y > 550) {
						game.buttonPressed(event.mouseButton.x / 200);
					}
    			}
			}
		}

		window.clear();
		game.update();

		window.draw(background);

		if (game.showEndscreen()) {
			endscreen.draw(&window, &font, game.getPlayerMoney());
			window.display();
			continue;
		}

		// draw ui
		window.draw(topbar);
		window.draw(bottombar);

		int curX = 0;
		betText.setString("Bet $" + to_string(game.getPlayerBet()));
		window.draw(betText);
		curX += betText.getLocalBounds().width + 30;
		int betSplit = game.getPlayerBetSplit();
		if (betSplit != 0) {
			splitText.setPosition(curX, -8);
			splitText.setString("Split $" + to_string(betSplit));
			window.draw(splitText);
		}
		curX += splitText.getLocalBounds().width + 30;		
		int bust = game.getPlayerBust();
		if (bust != 0) {
			bustText.setPosition(curX, -8);
			bustText.setString("Bust $" + to_string(bust));
			window.draw(bustText);
		}
		moneyText.setString("Balance $" + to_string(game.getPlayerMoney()));
		moneyText.setPosition(800 - moneyText.getLocalBounds().width - 10 ,-8);
		window.draw(moneyText);

		string* choices = game.getPlayerChoices();
		for (int i = 0; i < 4; i++) {
			buttonText.setString(choices[i]);
			buttonText.setPosition(-5+i*200+(200-buttonText.getLocalBounds().width)/2, 550);
			window.draw(buttonText);
		}
		delete[] choices;

		game.draw(&window, &toastText, &cardtext);
		
		if (game.isBettingActive()) {
			window.draw(betdialog);
		}

		window.display();
	}

	return 0;
}
