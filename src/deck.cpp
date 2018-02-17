#include <vector>
#include <algorithm>

using namespace std;

class Deck {
    vector<int> cards;
  public:
    void shuffle();
    int draw();
    void reset();
    Deck(void);
};

Deck::Deck(void) {
    reset();
}

void Deck::reset() {
    srand(time(0));
    cards.clear();
    for (int c = 0; c < 13; c++) {
        for (int i = 0; i < 4*6; i++) {
            cards.push_back(c);
        }
    }
    shuffle();
}

void Deck::shuffle() {
    random_shuffle(cards.begin(), cards.end());
}

int Deck::draw() {
    int ret = cards.back();
    cards.pop_back();
    return ret;
}