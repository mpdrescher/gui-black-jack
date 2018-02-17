# This file is for building an executable with embedded images and font
xxd -i assets/Beholder.ttf > src/resHeader/fontHeader.hpp
xxd -i assets/background.png > src/resHeader/resBackground.hpp
xxd -i assets/betdialog.png > src/resHeader/resBetdialog.hpp
xxd -i assets/cards.png > src/resHeader/resCards.hpp
xxd -i assets/overlay.png > src/resHeader/resOverlay.hpp
g++ -c src/mainCompact.cpp -o blackjack.o
g++ blackjack.o -o blackjack -lsfml-graphics -lsfml-window -lsfml-system