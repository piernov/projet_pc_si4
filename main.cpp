#include <iostream>
#include <array>
#include <vector>
#include <random>
#include <ctime>
#include <cmath>
#include <thread>
#include <shared_mutex>

#define LINES 128
#define COLUMNS 512

typedef std::array<std::array<bool, COLUMNS>, LINES> Map;

static std::shared_mutex mt;

void display_map(const auto &map);


std::array<std::pair<int, int>, 2> map_exit = {{ { 0, 0 }, { 1, 0} }};

enum class Direction {
	N, NE, E, SE, S, SW, W, NW 
};

std::pair<int, int> move_person(int x, int y, Direction d) {
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

auto compute_direction(const Map &map, int x, int y) {
	auto [exit_x, exit_y] = map_exit.at(0);

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


//	auto direction = Direction::N;
//	return direction;
}

void thread_main(Map &map, int line, int column) {
	while (line != 0 /*|| column > 2*/) { // algorithm only go north for now
		auto direction = compute_direction(map, column, line);

		auto [newcolumn, newline] = move_person(column, line, direction);

		{
			std::lock_guard lk(mt); // write-lock
			auto &newcell = map.at(newline).at(newcolumn);
			if (newcell) continue;
			std::swap(map.at(line).at(column), newcell);
			std::cout << "line: " << line << ", column: " << column << std::endl;
		}
		{
			std::lock_guard lk(mt);
			display_map(map);
		}

		line = newline;
		column = newcolumn;
	}
	{ // We're at the exit
		std::lock_guard lk(mt); // write-lock
		map.at(line).at(column) = false;
		std::cout << "Thread finished!" << std::endl;
	}
}

void init_map(auto &map, int p) {
	auto seed = time(0);
	auto people = 1 << p;
	std::cout << "Seed: " << seed << ", people = " << people << std::endl;
	std::srand(seed);

	for (auto i = 0; i < people; i++) {
		auto line = std::rand() % LINES;
		auto column = std::rand() % COLUMNS;
		if (!map.at(line).at(column))
			map[line][column] = true;
	}
}

void init_threads(auto &threads, auto &map) {
	for (auto i = 0; i < map.size(); i++) {
		auto line = map[i]; // no boundary check, for loop shouldn't go over
		for (auto j = 0; j < line.size(); j++) {
			auto cell = line[j]; // same
			if (cell)
				threads.emplace_back(thread_main, std::ref(map), i, j);
		}
	}
}

void display_map(const auto &map) {
	std::cout << "#  ";
	for (auto i = 0; i < COLUMNS - 1; i++) std::cout << '#';
	std::cout << std::endl;

	{
		//std::shared_lock lk(mt); // read-lock

		for (const auto &line: map) {
			std::cout << '#';
			for (const auto &cell: line) {
				if (cell) std::cout << 'x';
				else std::cout << '.';
			}
			std::cout << '#' << std::endl;
		}
	}

	for (auto i = 0; i < COLUMNS + 2; i++) std::cout << '#';
	std::cout << std::endl;
}

int main(int argc, char* argv[]) {
	// Map declaration
	std::array<std::array<bool, COLUMNS>, LINES> map{};

	// Command line parameters parsing
	bool benchmark_mode = false;
	int threads_mode = 0;
	int person_count = 0;

	for (int i = 0; i < argc; i++) {
		auto arg = argv[i];
		if (arg[0] != '-') continue;
		switch (arg[1]) {
			case 'p':
				person_count = arg[2] - '0';
				break;
			case 't':
				threads_mode = arg[2] - '0';
				break;
			case 'm':
				benchmark_mode = true;
				break;
			default:
				continue;
		}
	}

	std::cout << "Init: benchmark_mode = " << benchmark_mode << ", threads_mode = " << threads_mode << ", person_count = " << person_count << std::endl;

	// Thread pool
	const auto threads_count = 1 << person_count;
	std::vector<std::thread> threads;
	threads.reserve(threads_count);



	init_map(map, person_count);
	init_threads(threads, map);
	//display_map(map);

	for (auto &thread: threads) {
		thread.join();
	}

	return 0;
}
