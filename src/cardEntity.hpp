#ifndef __CARD
#define __CARD

#include <math.h>

const int START_POS_X = 400;
const int START_POS_Y = -100;

const float ANIMATION_SPEED = 9.0;

class CardEntity {
    int value;
    int posX;
    int posY;
    float posXF;
    float posYF;
    int destX;
    int destY;
    int delta();
    float deltaX();
    float deltaY();
  public:
    CardEntity(int, int, int);
    void update();
    void setDest(int, int);
    void setValue(int);
    int getValue();
    int getX();
    int getY();
};

#endif