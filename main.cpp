#include <iostream>
#include <vector>
#include <random>
#include <ctime>
#include <thread>
#include <mutex>

#include "Map.h"

static std::mutex mt;

void thread_main(Map &map, Person &person) {
	int column = person.getX();
	int line = person.getY();

	while (line != 0 || column > 2) { // algorithm only go north for now
		auto direction = map.computeDirection(column, line);

		auto newperson = map.movePerson(column, line, direction);
		auto newcolumn = newperson.first;
		auto newline = newperson.second;

		{
			Space* oldcell = map.getCell(column, line);
			Space* newcell = map.getCell(newcolumn, newline);
			if (newcell != nullptr)
				newcell->arrive();
			if (oldcell != nullptr)
				oldcell->depart();
			person.setX(newcolumn);
			person.setY(newline);

			line = person.getY();
			column = person.getX();
			std::cout << "line: " << line << ", column: " << column << std::endl;
		}
		{
			std::lock_guard<std::mutex> lk(mt);
			map.print();
		}


	}
	{ // We're at the exit
		Space* oldcell = map.getCell(column, line);
		if (oldcell != nullptr)
			oldcell->depart();
		person.setX(0);
		person.setY(0);
		std::cout << "Thread finished!" << std::endl;
	}
}

void init_threads(std::vector<std::thread> &threads, Map &map) {
	std::cout << map.getPeople().size() << std::endl;
	for (auto &person: map.getPeople()) {
		std::cout << "ee" << std::endl;
		threads.emplace_back(thread_main, std::ref(map), std::ref(person));
	}
}

int main(int argc, char* argv[]) {
	// Map declaration
	Map map;

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


	map.init(person_count);
	init_threads(threads, map);
	//display_map(map);

	for (auto &thread: threads) {
		thread.join();
	}

	return 0;
}
