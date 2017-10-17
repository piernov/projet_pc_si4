#include "Map.h"

#include <iostream>
#include <algorithm>
#include <cmath>

Map::Map() {
}

std::vector<Person> Map::init(int sqrtpeople) {
	auto seed = time(0);
	auto people_count = 1 << sqrtpeople; // 2^sqrtpeople
	std::cout << "Seed: " << seed << ", people = " << people_count << std::endl;
	std::srand(seed);

	for (auto i = 0; i < people_count; i++) {
		auto line = std::rand() % lines;
		auto column = std::rand() % columns;

		Person person = {column, line};
		if (std::find(people.begin(), people.end(), person) == people.end()) {
			people.emplace_back(person);
			map[line][column].arrive();
		}
	}

	return people;
}

void Map::print() {
	std::cout << "#  ";
	for (auto i = 0; i < columns - 1; i++) std::cout << '#';
	std::cout << std::endl;

	{
		//std::shared_lock lk(mt); // read-lock unused

		for (auto i = 0; i < map.size(); i++) {
			std::cout << '#';

			for (auto j = 0; j < map[i].size(); j++) {
				Person person = {i, j};
				if (std::find(people.begin(), people.end(), person) != people.end())
					std::cout << 'x';
				else
					std::cout << '.';
			}
			std::cout << '#' << std::endl;
		}
	}

	for (auto i = 0; i < columns + 2; i++) std::cout << '#';
	std::cout << std::endl;
}

std::pair<int, int> Map::movePerson(int x, int y, Direction d) {
	switch (d) {
		case Direction::N:
			return {x, y-1};
		case Direction::NE:
			return {x+1, y-1};
		case Direction::E:
			return {x+1, y};
		case Direction::SE:
			return {x+1, y+1};
		case Direction::S:
			return {x, y+1};
		case Direction::SW:
			return {x-1, y+1};
		case Direction::W:
			return {x-1, y};
		case Direction::NW:
			return {x-1, y-1};
		default:
			return {x, y};
	}
}

Cell &Map::getCell(int x, int y) {
	return map.at(y).at(x);
}

Direction Map::computeDirection(int x, int y) {
	// Handle only one exit pixel
	auto exit0 = mapExit.at(0);
	auto exit_x = exit0.first;
	auto exit_y = exit0.second;

	float angle = std::atan2(y - exit_y, x - exit_x) * 180 / 3.14159;
	std::cout << "x: " << x << "y: " << y << " angle: " << angle << " card: " << angle/45 << std::endl;
	auto card = static_cast<int>(std::round(angle/45));
	std::cout << "card : " << card << std::endl;

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
