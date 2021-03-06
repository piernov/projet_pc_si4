#ifndef _MAP_H_
#define _MAP_H_

#include <array>
#include <vector>

#include "Cell.h"
#include "Direction.h"
#include "Person.h"
#include "Space.h"

class Map {
public:
	Map();
	void init(int people_count);
	void initV2(int people_count);
	void communInit(int people_count);
	std::vector<Person*> getPeople(int i);
	std::vector<Person*> getPeople();
	std::pair<int, int> movePerson(int x, int y, int d);
	int checkDirection(int x, int y, int d);
	Space * getCell(int x, int y);
	int computeDirection(int x, int y);
	std::pair<int, int> getNextPosition(int column, int line);
	void makeWall(int length, int width, int x, int y);

	int getTID(std::pair<int, int> location) const;
	int getPeopleCount() const;

	void print();
private:
	static const int lines = 128;
	static const int columns = 512;

	std::vector<std::vector<Space*>> map;
	std::vector<Person> people{};
	std::array<std::pair<int, int>, 2> mapExit = {{ { 0, 0 }, { 1, 0} }};

	std::array<std::vector<Person *>, 4> peopleSections = {};

};

#endif
