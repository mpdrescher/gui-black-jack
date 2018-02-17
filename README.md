# Black Jack

This is a Black Jack implementation using SFML.

### Screenshot

![Screenshot](/screenshot.png?raw=true)

### Build and Run

Since the used font (see `fontcredits`) is not mine and its redistribution is not permitted you have to download it yourself by running the script `fetchFont.sh` from the main directory.  
After that just run `make` to build and run.  
There is also the `buildCompact.sh` script that generates headers in `src/resHeader` and builds a standalone executable.

### Description

Besides standard Black Jack rules the following apply:
- You may bet on dealer bust on round start, the win ratio is 5:2
- On the first round you may split the cards if they are the same (and you have enough money) or double your bet
- The simulated card deck consists of 6 times 52 cards and is shuffled before each round
- If your balance reaches 0$ you get a game over
- Before each round you can decide to cash out which ends the game and places your score in the highscore list (top 3)
- The number card "10" has the label "X" because of typesetting purposes