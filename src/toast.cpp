Toast::Toast(int x, int y, string message) {
    msg = message;
    posX = x;
    posY = y;
    opacity = 0;
}

bool Toast::shouldDestroy() {
    return opacity >= 255;
}

void Toast::update() {
    posY -= 1;
    opacity += 3;
}

void Toast::draw(RenderWindow *window, Text *sprite) {
    float opacityFloat = ((float) (255-opacity))/255.0;
    int opacitySqrt = (int) (sqrt(opacityFloat)*255.0);
    sprite -> setString(msg);
    sprite -> setPosition(posX- (sprite -> getLocalBounds()).width/2, posY);
    sprite -> setColor(Color(255, 0, 0, opacitySqrt));
    window -> draw(*sprite);
}



ToastManager::ToastManager() {

}

void ToastManager::addToast(string msg) {
    Toast toast(400, 400, msg);
    toasts.push_back(toast);
}

void ToastManager::update() {
    for (vector<Toast>::iterator iter = toasts.begin(); iter != toasts.end(); ++iter) {
        iter -> update();
    }
    // is this the right way to do this??
    int curIndex = 0;
    while (curIndex < toasts.size()) {
        if (toasts.at(curIndex).shouldDestroy()) {
            toasts.erase(toasts.begin()+curIndex);
        }
        else {
            curIndex ++;
        }
    }
}

void ToastManager::draw(RenderWindow *window, Text *sprite) {
    for (vector<Toast>::iterator iter = toasts.begin(); iter != toasts.end(); ++iter) {
        iter -> draw(window, sprite);
    }
}