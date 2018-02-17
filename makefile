all:
	g++ -c src/main.cpp -o blackjack.o
	g++ blackjack.o -o blackjack -lsfml-graphics -lsfml-window -lsfml-system
	./blackjack
