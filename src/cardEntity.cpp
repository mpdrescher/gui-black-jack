#include "cardEntity.hpp"

CardEntity::CardEntity(int val, int x, int y) {
    value = val;
    destX = x;
    destY = y;
    posX = START_POS_X;
    posY = START_POS_Y;
    posXF = START_POS_X;
    posYF = START_POS_Y;
}

void CardEntity::update() {
    float stepX = deltaX()/ANIMATION_SPEED;
    float stepY = deltaY()/ANIMATION_SPEED;
    posXF += stepX;
    posYF += stepY;
    posX = (int) posXF;
    posY = (int) posYF;
}

float CardEntity::deltaX() {
    float destXF = (float) destX;
    return destXF - posXF;
}

float CardEntity::deltaY() {
    float destYF = (float) destY;
    return destYF - posYF;
}

int CardEntity::delta() {
    float dX = deltaX();
    float dY = deltaY();
    float dist = sqrt(dX*dX + dY*dY);
    return (int) dist;
}

void CardEntity::setDest(int x, int y) {
    destX = x;
    destY = y;
}

void CardEntity::setValue(int val) {
    value = val;
}

int CardEntity::getValue() {
    return value;
}

int CardEntity::getX() {
    return posX;
}

int CardEntity::getY() {
    return posY;
}