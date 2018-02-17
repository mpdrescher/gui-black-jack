#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <iterator>
#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

const string SAVE_FILE = "highscores.dat";

class Endscreen {
    int scores[3];
    int scoresSaved;
  public:
    Endscreen();
    void draw(RenderWindow*, Font* font, int);
    void saveToDisk(int[4]);
};

Endscreen::Endscreen() {
    scoresSaved = false;
    for (int i = 0; i < 3; i++) {
        scores[i] = 0;
    }
    string line;
    ifstream file(SAVE_FILE);
    if (file.is_open()) {
        int curIndex = 0;
        while (getline(file, line)) {
            scores[curIndex] = stoi(line);
            curIndex ++;
            if (curIndex == 3) {
                break;
            }
        }
        file.close();
    }
}

void Endscreen::saveToDisk(int scores[4]) {
    scoresSaved = true;
    ofstream file(SAVE_FILE);
    if (file.is_open()) {
        file << to_string(scores[0]) << endl;
        file << to_string(scores[1]) << endl;
        file << to_string(scores[2]) << endl;
        file.close();
    }
    else {
        cout << "Unable to open file";
    }
}

void Endscreen::draw(RenderWindow *window, Font *font, int playerMoney) {
    Text hsText;
    hsText.setFont(*font);
    hsText.setCharacterSize(50);
    hsText.setString("Highscores:");
    hsText.setPosition(400-hsText.getLocalBounds().width/2, 50);

    int scoresPlus[4];
    for (int i = 0; i < 3; i++) {
        scoresPlus[i] = scores[i];
    }
    scoresPlus[3] = playerMoney;
    sort(scoresPlus, scoresPlus + 4);
    reverse(begin(scoresPlus), end(scoresPlus));
    if (scoresSaved == false) {
        saveToDisk(scoresPlus);
    }

    bool wasCurrentShown = false;
    int curNumber = 1;
    for (int i = 0; i < 4; i++) {
        Text currentLabel;
        Text currentValue;
        currentLabel.setFont(*font);
        currentValue.setFont(*font);
        currentLabel.setCharacterSize(40);
        currentValue.setCharacterSize(40);
        if (wasCurrentShown || (wasCurrentShown == false && scoresPlus[i] != playerMoney)) {
            if (scoresPlus[i] == 0) {
                continue;
            }
            currentLabel.setString(to_string(curNumber) + ".");
            curNumber ++;
        }
        else {
            if (i == 3) {
                currentLabel.setString("--");
            }
            else {
                currentLabel.setString("new!");
            }
            currentLabel.setColor(Color::Yellow);
            currentValue.setColor(Color::Yellow);
            wasCurrentShown = true;
        }
        currentValue.setString("$" + to_string(scoresPlus[i]));
        currentLabel.setPosition(330, 175+i*60);
        currentValue.setPosition(430, 175+i*60);
        window -> draw(currentLabel);
        window -> draw(currentValue);
    }

    if (playerMoney == 0) {
        Text brokeText;
        brokeText.setFont(*font);
        brokeText.setCharacterSize(40);
        brokeText.setColor(Color::Black);
        brokeText.setString("you are broke!");
        brokeText.setPosition(400 - brokeText.getLocalBounds().width / 2, 475);
        window -> draw(brokeText);
    }

    window -> draw(hsText);
}