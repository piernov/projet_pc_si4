#ifndef _MAP_H_
#define _MAP_H_

#include <array>
#include <vector>

#include "Direction.h"

#define LINES 128
#define COLUMNS 512

class Map {
public:
	Map();
	void init(int sqrtpeople);
	std::vector<std::pair<int, int>> getPeople();
	std::pair<int, int> movePerson(int x, int y, Direction d);
	void move(bool &from, bool &to);
	bool &getCell(int x, int y);
	Direction computeDirection(int x, int y);

	void print();
private:
	std::array<std::array<bool, COLUMNS>, LINES> map{};
	std::array<std::pair<int, int>, 2> mapExit = {{ { 0, 0 }, { 1, 0} }};

};

#endif
