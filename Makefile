pacman: pacman.cpp
	g++ -g -O3 pacman.cpp -o pacman -DNDEBUG=1

pac3: pacman.cpp
	g++ -g -O3 pacman.cpp -o pacman -DNDEBUG=1 -DPAC3=1

all: pacman pac3

