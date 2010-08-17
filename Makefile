pacman: pacman.cpp
	g++ -g -O3 pacman.cpp -o pacman -DNDEBUG=1

pacmand: pacman.cpp
	g++ -g -O2 pacman.cpp -o pacmand

pac3: pacman.cpp
	g++ -g -O3 pacman.cpp -o pac3 -DNDEBUG=1 -DPAC3=1

all: pacman pac3 pacmand
