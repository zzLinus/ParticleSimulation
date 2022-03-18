CPPFLAGS = -lX11 -lGL -lpthread -lpng -lstdc++fs -std=c++17

SandSimu: main.o
	g++ main.o -o SandSimu $(CPPFLAGS)

main.o: main.cpp
	g++ -c main.cpp -o main.o $(CPPFLAGS)
