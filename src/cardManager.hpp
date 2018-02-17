#ifndef __CARDMANAGER
#define __CARDMANAGER

#include "cardEntity.hpp"
#include <vector>
#include <SFML/Graphics.hpp>
#include <iostream>

using namespace sf;

const int CLEANUP_DELAY = 60;

const int CLEANUP_POS_X = -50;
const int CLEANUP_POS_Y = 300;

class CardManager {
    vector<CardEntity> cards;
    int anchorX;
    int anchorY;
    int cleanupDelay;
  public:
    CardManager();
    void update();
    void pushCard(int);
    void clear();
    void recalcLayout();
    void setAnchor(int, int);
    void draw(RenderWindow*, Texture*);
    bool splitPossible();
    int cardCount();
    void replaceCard13(Deck*);
    bool hasBlackJack();
    int getFirstValue();
    void cleanup();
    void cleanupDelayed();
};

#endif