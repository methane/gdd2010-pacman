pacman: pac1.cpp
	g++ -g -O3 pac1.cpp -o pacman -DNDEBUG=1

all: pacman
