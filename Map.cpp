#include "Map.h"
#include "Cell.h"
#include "Wall.h"
#include "CellV2.h"

#include <iostream>
#include <algorithm>
#include <array>
#include <cmath>
#include <ctime>


Map::Map() {
}

int Map::getTID(std::pair<int, int> location) const {
	if (location.first < 256 && location.second < 128)
		return 0;
	else if (location.first >= 256 && location.second < 128)
		return 1;
	else if (location.first < 256 && location.second >= 128)
		return 2;
	else
		return 3;
}

std::vector<Person> Map::init(int sqrtpeople) {
	for (int i = 0; i < lines; ++i) {
		std::vector<Space *> vec;
		for (int j = 0; j < columns; ++j) {
			Cell *cell = new Cell();
			vec.push_back(cell);
		}
		map.push_back(vec);
	}
	communInit(sqrtpeople);
	return people;
}

std::vector<Person> Map::initV2(int sqrtpeople) {
	for (int i = 0; i < lines; ++i) {
		std::vector<Space *> vec;
		for (int j = 0; j < columns; ++j) {
			if (i == 63 || i == 64 || j == 255 || j == 256) {
				CellV2 *cell = new CellV2(true);
				vec.push_back(cell);
			} else {
				CellV2 *cell = new CellV2(false);
				vec.push_back(cell);
			}
		}
		map.push_back(vec);
	}
	communInit(sqrtpeople);
	return people;
}

void Map::communInit(int sqrtpeople) {
	makeWall(30, 27, 45, 45);
	makeWall(15, 40, 70, 14);
	makeWall(30, 30, 250, 50);
	makeWall(10, 10, 50, 1);
	auto seed = time(0);
	auto people_count = 1 << sqrtpeople; // 2^sqrtpeople
	std::cout << "Seed: " << seed << ", people = " << people_count << std::endl;
	std::srand(seed);

	while (people_count > 0) {
		auto line = std::rand() % lines;
		auto column = std::rand() % columns;

		Person person = {column, line};
		if (std::find(people.begin(), people.end(), person) == people.end() && !map[line][column]->isWall()) {
			people.emplace_back(person);
			people_count--;
			map[line][column]->arrive();

			peopleSections.at(getTID(std::pair<int, int>(column, line))).push_back(&person);
		}
	}
}


void Map::print() {
	std::cout << "#  ";
	for (auto i = 0; i < columns - 1; i++) std::cout << '#';
	std::cout << std::endl;

	{

		for (auto i = 0; i < map.size(); i++) {
			std::cout << '#';

			for (auto j = 0; j < map[i].size(); j++) {
				if (map[i][j] != nullptr)
					map[i][j]->print(std::cout);
			}
			std::cout << '#' << std::endl;
		}
	}

	for (auto i = 0; i < columns + 2; i++) std::cout << '#';
	std::cout << std::endl;
}

std::pair<int, int> Map::movePerson(int x, int y, int d) {
	switch (d) {
		case Direction::N :
			return {x, y - 1};
		case Direction::NW :
			return {x - 1, y - 1};
		case Direction::W :
			return {x - 1, y};
		default:
			return {x, y};
	}
}

Space *Map::getCell(int x, int y) {
	return map.at(y).at(x);
}

int Map::computeDirection(int x, int y) {
	// Handle only one exit pixel
	auto exit0 = mapExit.at(0);
	auto exit_x = exit0.first;
	auto exit_y = exit0.second;

	float angle = std::atan2(y - exit_y, x - exit_x) * 180 / 3.14159;
	auto card = static_cast<int>(std::round(angle / 45));

	switch (card) {
		case 0:
			return Direction::W;
		case 1:
			return Direction::NW;
		case 2:
			return Direction::N;
		default:
			return Direction::N; // Shouldn't be something else since exit is at top-left corner
	}
}

std::vector<Person> &Map::getPeople() {
	return people;
}

int Map::checkDirection(int x, int y, int d) {
	for (;; d = (d + 1) % 3) {
		std::pair<int, int> coor = movePerson(x, y, d);
		Space *space = map[coor.second][coor.first];
		if (space != nullptr && !space->isWall())
			break;
	}
	return d;
}

std::pair<int, int> Map::getNextPosition(int column, int line) {
	auto direction = computeDirection(column, line);
	direction = checkDirection(column, line, direction);
	return movePerson(column, line, direction);
}

void Map::makeWall(int length, int width, int x, int y) {
	for (int i = y; i < map.size() && i < y + length; ++i) {
		for (int j = x; j < map.at(i).size() && j < x + width; ++j) {
			Wall *wall = new Wall();
			map.at(i).at(j) = wall;
		}
	}

}

