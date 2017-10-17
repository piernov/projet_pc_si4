#ifndef _MAP_H_
#define _MAP_H_

#include <array>
#include <vector>

#include "Cell.h"
#include "Direction.h"
#include "Person.h"

class Map {
public:
	Map();
	std::vector<Person> init(int sqrtpeople);
	std::vector<Person> &getPeople();
	std::pair<int, int> movePerson(int x, int y, Direction d);
	Cell &getCell(int x, int y);
	Direction computeDirection(int x, int y);

	void print();
private:
	static const int lines = 128;
	static const int columns = 512;

	std::array<std::array<Cell, columns>, lines> map{};
	std::vector<Person> people{};
	std::array<std::pair<int, int>, 2> mapExit = {{ { 0, 0 }, { 1, 0} }};

};

#endif
