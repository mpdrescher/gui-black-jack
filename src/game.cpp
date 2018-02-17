#include <string>
#include <iostream>
#include "toast.hpp"
#include "cardEntity.hpp"
#include "cardManager.hpp"

// The game state constants
const int GS_STARTOFROUND = 0; // the first screen (bet, bet on bust, all-in)
const int GS_STARTOFROUND_BUST = 1; // the first screen without bet on bust
const int GS_BETTING = 2; // the betting screen
const int GS_BETTING_BUST = 3; // the dealer bust betting screen
const int GS_FIRSTROUND = 4;
const int GS_PLAYERTURN = 5; // active the whole time the player can do something (except for the first round)
const int GS_AITURN = 6; // dealer turn
const int GS_EVAL_SPLIT = 10; // hack gamestate so that the split hand evaluation is animated
const int GS_FINISH = 7; // animation state
const int GS_GAMEOVER = 8; // active while the endscreen is showing
const int GS_BETTING_SPLIT = 9; // bet on the split hand

const int AS_REPLACE13 = 0; // indicates wether the ai has turned the second dealer card on its first turn (card value 13)
const int AS_DRAW = 1;

const int AI_SPEED = 75; // in Ticks

const int CLEANUP_TIMEOUT = 60; // animation speed on gs_finish and gs_eval_split

// value of the buttons on the betting screen
static int betButtonValue(int row, int col) {
    if (row == 0 && col == 0) {
        return 5;
    }
    else if (row == 0 && col == 1) {
        return 25;
    }
    else if (row == 0 && col == 2) {
        return 100;
    }
    else if (row == 1 && col == 0) {
        return 250;
    }
    else if (row == 1 && col == 1) {
        return 1000;
    }
    else if (row == 1 && col == 2) {
        return -1;
    }
}

class Game {
    Deck deck;
    int playerMoney;
    int bet;
    int bust; // 0 indicates inactive
    int betSplit; // 0 when inactive
    int gameState;
    int aiTimer;
    int aiState;
    CardManager playerCards;
    CardManager splitCards;
    CardManager dealerCards;
    ToastManager toastManager;
  public:
    Game(void);
    void resetAll(); // game reset
    void newRound(); // round reset
    void dealerDraw(bool); // dealer draw (bool true -> conceal the card)
    void playerDraw();
    void splitDraw();
    void makeAIMove(); // the ai function
    void initAI(); // ai init
    void allIn();
    int getPlayerBetSplit();
    int getPlayerBet();
    int getPlayerMoney();
    int getPlayerBust();
    bool isBettingActive(); // wether the betting dialog should show
    void finishGame(); // game end (usually after ai turn or player bust)
    void playStartHand(); // 2 dealer cards, 2 player cards are given
    void buttonPressed(int); // action button event, button numbers are 0 to 3 from left to right
    void initSplit();
    void evaluateHand(int, string, int, bool); // final hand evaluation
    void setBet(int, int);
    void update(); // update animations
    void draw(RenderWindow*, Text*, Texture*); // draw components like toasts and cards
    bool splitActive();
    bool showEndscreen();
    ToastManager* getToastManager();
    string* getPlayerChoices(); // returns the strings displayed on the action buttons
};

Game::Game(void) {
    playerCards.setAnchor(400, 450);
    splitCards.setAnchor(400, 375);
    dealerCards.setAnchor(400, 150);
    resetAll();
}

void Game::resetAll() {
    newRound();
    playerMoney = 100;
}

void Game::newRound() {
    deck.reset();
    playerCards.clear();
    splitCards.clear();
    dealerCards.clear();
    bet = 0;
    bust = 0;
    betSplit = 0;
    gameState = GS_STARTOFROUND;
    aiTimer = AI_SPEED;
    aiState = 0;
}

void Game::dealerDraw(bool concealed) {
    if (concealed) {
        dealerCards.pushCard(13);
    }
    else {
        dealerCards.pushCard(deck.draw());
    }
}

void Game::playerDraw() {
    playerCards.pushCard(deck.draw());
}

void Game::splitDraw() {
    splitCards.pushCard(deck.draw());
}

void Game::update() {
    dealerCards.update();
    playerCards.update();
    splitCards.update();

    if (gameState == GS_AITURN) {
        aiTimer --;
        if (aiTimer == 0) {
            makeAIMove();
            aiTimer = AI_SPEED;
        }
    }
    if (gameState == GS_EVAL_SPLIT) {
        aiTimer --;
        if (aiTimer == 0) {
            int splitScore = splitCards.cardCount();
            bool splitBJ = splitCards.hasBlackJack();
            evaluateHand(betSplit, "split", splitScore, splitBJ);
            aiTimer = CLEANUP_TIMEOUT;
            gameState = GS_FINISH;
            return;
        }
    }
    if (gameState == GS_FINISH) {
        aiTimer --;
        if (aiTimer == CLEANUP_TIMEOUT / 2) {
            playerCards.cleanup();
            splitCards.cleanup();
            dealerCards.cleanup();
        }
        if (aiTimer == 0) {
            if (playerMoney == 0) {
                gameState = GS_GAMEOVER;
                return;
            }
            newRound();
        }
    }
}

bool Game::showEndscreen() {
    return gameState == GS_GAMEOVER;
}

void Game::makeAIMove() {
    if (aiState == AS_REPLACE13) {
        aiState = AS_DRAW;
        dealerCards.replaceCard13(&deck);
    }
    else if (aiState == AS_DRAW) {
        if (dealerCards.cardCount() <= 16) {
            dealerDraw(false);
        }
        else {
            finishGame();
        }
    }
}

void Game::finishGame() {
    gameState = GS_FINISH;
    aiTimer = CLEANUP_TIMEOUT;
    int playerScore = playerCards.cardCount();
    bool playerBJ = playerCards.hasBlackJack();
    evaluateHand(bet, "player", playerScore, playerBJ);
    if (splitActive()) {
        gameState = GS_EVAL_SPLIT; // overriding on purpose
    }
}

void Game::evaluateHand(int betValue, string prefix, int handScore, bool handBJ) {
    int dealerScore = dealerCards.cardCount();
    bool dealerBJ = dealerCards.hasBlackJack();    
    if (handScore > 21) {
        toastManager.addToast(prefix + ": bust");
    }
    else if (handBJ && dealerBJ) {
        toastManager.addToast(prefix + ": black jack tie");
        playerMoney += betValue;
    }
    else if (handBJ) {
        toastManager.addToast(prefix + ": black jack");
        playerMoney += betValue;
        playerMoney += betValue*3/2;
    }
    else if (dealerScore > 21) {
        if (bust == 0) {
            toastManager.addToast(prefix + ": dealer bust");
            playerMoney += 2*betValue;
        }
        else {
            toastManager.addToast(prefix + ": dealer bust + bet bonus");
            playerMoney += 2*betValue;
            playerMoney += bust*5/2;
            bust = 0;
        }
    }
    else if (handScore == dealerScore) {
        toastManager.addToast(prefix + ": tie");
        playerMoney += betValue;
    }
    else if (handScore > dealerScore) {
        toastManager.addToast(prefix + ": win");
        playerMoney += betValue*2;
    }
    else if (dealerScore > handScore) {
        toastManager.addToast(prefix + ": lose");
    }
}

int Game::getPlayerBet() {
    return bet;
}

int Game::getPlayerMoney() {
    return playerMoney;
}

int Game::getPlayerBust() {
    return bust;
}

string* Game::getPlayerChoices() {
    string* choices = new string[4];
    for (int i = 0; i < 4; i++) {
        choices[i] = "";
    }
    if (gameState == GS_STARTOFROUND) {
        choices[0] = "Place Bet";
        choices[1] = "Bet on Bust";
        choices[2] = "All-In";
        choices[3] = "Cashout";
    }
    if (gameState == GS_STARTOFROUND_BUST) {
        choices[0] = "Place Bet";
        choices[2] = "All-In";
    }
    if (gameState == GS_FIRSTROUND) {
        choices[0] = "Split";
        choices[1] = "Double";
        choices[3] = "Nothing";
    }
    if (gameState == GS_PLAYERTURN) {
        choices[0] = "Hit";
        if (splitActive()) {
            choices[1] = "Hit Split";
        }
        choices[3] = "Stand";
    }
    return choices;
}

void Game::buttonPressed(int no) {
    if (no == 0 && gameState == GS_STARTOFROUND) {
        if (playerMoney >= 5) {
            gameState = GS_BETTING;
        }
        else {
            toastManager.addToast("you need to go all-in");
        }
    }
    if (no == 1 && gameState == GS_STARTOFROUND) {
        if (playerMoney >= 10) {
            gameState = GS_BETTING_BUST;
        }
        else {
            toastManager.addToast("not enough money");
        }
    }
    if (no == 2 && gameState == GS_STARTOFROUND) {
        allIn();
    }
    if (no == 3 && gameState == GS_STARTOFROUND) {
        gameState = GS_GAMEOVER;
    }

    if (no == 0 && gameState == GS_STARTOFROUND_BUST) {
        if (playerMoney >= 5) {
            gameState = GS_BETTING;
        }
        else {
            toastManager.addToast("you need to go all-in");
        }
    }
    if (no == 2 && gameState == GS_STARTOFROUND_BUST) {
        allIn();
    }

    if (no == 0 && gameState == GS_FIRSTROUND) { // split
        if (playerCards.splitPossible() && playerMoney >= 5) {
            gameState = GS_BETTING_SPLIT;
        }
        else if (playerCards.splitPossible()) {
            toastManager.addToast("not enough money");
        }
        else {
            toastManager.addToast("requires two identical cards");
        }
    }
    if (no == 1 && gameState == GS_FIRSTROUND) { // double
        if (bet <= playerMoney) {
            playerMoney -= bet;
            bet *= 2;
            gameState = GS_PLAYERTURN;
        }
        else {
            toastManager.addToast("not enough money");
        }
    }
    if (no == 3 && gameState == GS_FIRSTROUND) { // nothing
        gameState = GS_PLAYERTURN;
        return;
    }

    if (no == 0 && gameState == GS_PLAYERTURN) {
        if (playerCards.cardCount() <= 21) {
            playerDraw();
        }
    }
    if (no == 1 && splitActive() && gameState == GS_PLAYERTURN) {
        if (splitCards.cardCount() <= 21) {
            splitDraw();
        }
    }
    if (no == 3 && gameState == GS_PLAYERTURN) { // finish
        gameState = GS_AITURN;
    }
    if (gameState == GS_PLAYERTURN) { // some game logic
        if (splitActive()) {
            if (playerCards.cardCount() > 21 && splitCards.cardCount() > 21) {
                finishGame();
            }
            else if (playerCards.cardCount() > 21) {
                playerCards.cleanupDelayed();
            }
            else if (splitCards.cardCount() > 21) {
                splitCards.cleanupDelayed();
            }
        }
        else if (playerCards.cardCount() > 21) {
            finishGame();
        }
    }
}

bool Game::isBettingActive() {
    return (gameState == GS_BETTING || gameState == GS_BETTING_BUST || gameState == GS_BETTING_SPLIT);
}

void Game::playStartHand() {
    playerDraw();
    playerDraw();
    dealerDraw(true);
    dealerDraw(false);
}

void Game::initSplit() {
    int splitValue = playerCards.getFirstValue();
    playerCards.clear();
    playerCards.pushCard(splitValue);
    splitCards.pushCard(splitValue);
    playerDraw();
    splitDraw();
}

void Game::allIn() {
    bet = playerMoney;
    playerMoney = 0;
    gameState = GS_FIRSTROUND;
    playStartHand();
}

void Game::setBet(int buttonRow, int buttonCol) {
    if (gameState == GS_BETTING) {
        int value = betButtonValue(buttonRow, buttonCol);
        if (value == -1) {
            if (bet > 0) {
                gameState = GS_FIRSTROUND;
                playStartHand();
            }
            else {
                toastManager.addToast("place a bet first");
            }
        }
        else if (value <= playerMoney) {
            bet += value;
            playerMoney -= value;
        }
        else {
            toastManager.addToast("not enough money");
        }
    }
    if (gameState == GS_BETTING_BUST) {
        int value = betButtonValue(buttonRow, buttonCol);
        if (value == -1) {
            if (bust > 0) {
                gameState = GS_STARTOFROUND_BUST;
            }
            else {
                toastManager.addToast("place a bet first");
            }
        }
        else if (value <= playerMoney && playerMoney >= value + 5) {
            bust += value;
            playerMoney -= value;
        }
        else {
            toastManager.addToast("not enough money");
        }
    }
    if (gameState == GS_BETTING_SPLIT) {
        int value = betButtonValue(buttonRow, buttonCol);
        if (value == -1) {
            if (betSplit > 0) {
                gameState = GS_PLAYERTURN;
                initSplit();
            }
            else {
                toastManager.addToast("place a bet first");
            }
        }
        else if (value <= playerMoney) {
            betSplit += value;
            playerMoney -= value;
        }
        else {
            toastManager.addToast("not enough money");
        }
    }
}

bool Game::splitActive() {
    return betSplit != 0;
}

int Game::getPlayerBetSplit() {
    return betSplit;
}

void Game::draw(RenderWindow *window, Text *toastSprite, Texture *cardTexture) {
    playerCards.draw(window, cardTexture);
    dealerCards.draw(window, cardTexture);
    splitCards.draw(window, cardTexture);
    toastManager.update();
    toastManager.draw(window, toastSprite);
}

ToastManager* Game::getToastManager() {
    ToastManager* ref = &toastManager;
    return ref;
}