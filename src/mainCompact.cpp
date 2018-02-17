/*
A COPY OF main.cpp FOR BUILDING WITH EMBEDDED FONT AND TEXTURES

PROGRAM LAYOUT:
==============

- "main.cpp"
	Set up SFML, load Resources and create game objects
- "game.cpp"
	The main class that manages the game state and processes the user input
- "deck.cpp"
	A simulated card deck
- "cardManager.cpp"
	The class that represents a players hand. In contrast to deck.cpp this file also handles the graphical representation of the cards and their layout on the table.
- "cardEntity.cpp"
	A card on the table with its value and position.
- "endscreen.cpp"
	The gameover screen and load/save functions.
- "toast.cpp"
	A graphical "Toast" (the red popup text) and class that manages them.
*/

#include <SFML/Graphics.hpp>
#include "deck.cpp"
#include "game.cpp"
#include "toast.cpp"
#include "cardEntity.cpp"
#include "cardManager.cpp"
#include "endscreen.cpp"

// include embedded resources
#include "resHeader/fontHeader.hpp"
#include "resHeader/resBackground.hpp"
#include "resHeader/resBetdialog.hpp"
#include "resHeader/resCards.hpp"
#include "resHeader/resOverlay.hpp"

using namespace sf;

const int TEXT_BACKGROUND = 0;
const int TEXT_CARDS = 1;
const int TEXT_BETDIALOG = 2;
const int TEXT_OVERLAY = 3;

// might seem overkill, but this makes embedding the resources in the compact version easier
void loadTexture(int textureConst, Texture *text) {
	if (textureConst == TEXT_BACKGROUND) {
		text -> loadFromMemory(&assets_background_png, assets_background_png_len);
	}
	else if (textureConst == TEXT_CARDS) {
		text -> loadFromMemory(&assets_cards_png, assets_cards_png_len);
	}
	else if (textureConst == TEXT_BETDIALOG) {
		text -> loadFromMemory(&assets_betdialog_png, assets_betdialog_png_len);
	}
	else if (textureConst == TEXT_OVERLAY) {
		text -> loadFromMemory(&assets_overlay_png, assets_overlay_png_len);
	}
}

void loadFont(Font *font) {
	font -> loadFromMemory(&assets_Beholder_ttf, assets_Beholder_ttf_len);
}

int main()
{
	RenderWindow window(VideoMode(800, 600), "Black Jack");
	window.setPosition(Vector2i(200,200));
	window.setFramerateLimit(60);
	
	// Init textures and Sprites
	Texture bgtext;
	loadTexture(TEXT_BACKGROUND, &bgtext);
	Texture cardtext;
	loadTexture(TEXT_CARDS, &cardtext);
	Texture bettingtext;
	loadTexture(TEXT_BETDIALOG, &bettingtext);	
	Texture uitext;
	loadTexture(TEXT_OVERLAY, &uitext);	
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
	loadFont(&font);
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

		// status bar information
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

		// action buttons
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
