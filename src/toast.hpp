#ifndef __TOAST
#define __TOAST

#include <SFML/Graphics.hpp>
#include <string>
#include <math.h>

using namespace sf;

class Toast {
    string msg;
    int posX;
    int posY;
    int opacity;
  public:
    Toast(int, int, string);
    bool shouldDestroy();
    void update();
    void draw(RenderWindow*, Text*);
};

class ToastManager {
    vector<Toast> toasts;
  public:
    ToastManager(void);
    void addToast(string);
    void update();
    void draw(RenderWindow*, Text*);
};

#endif