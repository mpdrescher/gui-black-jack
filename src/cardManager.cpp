// NOTICE: card values are from 0 to 13.
// 0 to 8 corresponds to the number cards 2 to 10
// 9 -> joker
// 10 -> queen
// 11 -> king
// 12 -> queen
//
// the card 13 is not available in the card deck and may only be used by the dealer to indicate a concealed card.
// the dealers hidden cards real value is only determined at the start of the AI move!

CardManager::CardManager() {
    cleanupDelay = 0;
}

void CardManager::update() {
    if (cleanupDelay > 0) {
        cleanupDelay --;
        if (cleanupDelay == 0) {
            cleanup();
        }
    }
    for (vector<CardEntity>::iterator iter = cards.begin(); iter != cards.end(); ++iter) {
        iter -> update();
    }
}

void CardManager::pushCard(int value) {
    CardEntity card(value, 0, 0); // x and y destination is done by the layout manager
    cards.push_back(card);
    recalcLayout();
}

void CardManager::recalcLayout() {
    int number = cards.size();
    int usedSpaceX = number * 40; // 5 px spacing on every card (35 px width), minus two outer cards
    int curX = anchorX - usedSpaceX / 2 + 20;//; - usedSpaceX / 2;
    for (vector<CardEntity>::iterator iter = cards.begin(); iter != cards.end(); ++iter) {
        iter -> setDest(curX, anchorY);
        curX += 40;
    }
}

void CardManager::clear() {
    cards.clear();
}

void CardManager::setAnchor(int x, int y) {
    anchorX = x;
    anchorY = y;
}

void CardManager::draw(RenderWindow *window, Texture *cardTexture) {
    Sprite card;
    card.setTexture(*cardTexture);
    for (vector<CardEntity>::iterator iter = cards.begin(); iter != cards.end(); ++iter) {
        int value = iter -> getValue();
        int x = iter -> getX();
        int y = iter -> getY();
        x -= 17;
        y -= 25;
        card.setTextureRect(IntRect(value*35, 0, 35, 50));
        card.setPosition(Vector2f(x, y));
		window -> draw(card);
    }
}

bool CardManager::splitPossible() {
    bool success = true;
    if (cards.size() != 2) {
        success = false;
    }
    CardEntity card1 = cards.at(0);
    CardEntity card2 = cards.at(1);
    if (card1.getValue() != card2.getValue()) {
        success = false;
    }
    return success;
}

bool CardManager::hasBlackJack() {
    if (cards.size() != 2) {
        return false; // prevent index oob
    }
    int card1 = cards.at(0).getValue();
    int card2 = cards.at(1).getValue();
    if ((card1 == 12 && card2 >= 8) || (card1 >= 8 && card2 == 12)) {
        return true;
    }
    return false;
}

int CardManager::cardCount() {
    int count = 0;
    int aces = 0;
    for (vector<CardEntity>::iterator iter = cards.begin(); iter != cards.end(); ++iter) {
        int value = iter -> getValue();
        if (value == 13) {
        }
        else if (value == 12) {
            aces ++;
        }
        else if (value == 11 || value == 10 || value == 9) {
            count += 10;
        }
        else {
            count += value + 2;
        }
    }
    for (int i = 0; i < aces; i++) {
        if (count + 11 <= 21) {
            count += 11;
        }
        else {
            count += 1;
        }
    }
    return count;
}

void CardManager::replaceCard13(Deck *deck) {
    for (vector<CardEntity>::iterator iter = cards.begin(); iter != cards.end(); ++iter) {
        int value = iter -> getValue();
        if (value == 13) {
            iter -> setValue(deck -> draw());
        }
    }
}

int CardManager::getFirstValue() {
    return cards.at(0).getValue();
}

void CardManager::cleanup() {
    for (vector<CardEntity>::iterator iter = cards.begin(); iter != cards.end(); ++iter) {
        iter -> setDest(CLEANUP_POS_X, CLEANUP_POS_Y);
    }
}

void CardManager::cleanupDelayed() {
    cleanupDelay = CLEANUP_DELAY;
}