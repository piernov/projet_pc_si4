#include <iostream>
#include <vector>
#include <random>
#include <ctime>
#include <thread>
#include <shared_mutex>

#include "Map.h"

static std::shared_mutex mt;

void thread_main(Map &map, int line, int column) {
	while (line != 0 /*|| column > 2*/) { // algorithm only go north for now
		auto direction = map.computeDirection(column, line);

		auto newperson = map.movePerson(column, line, direction);
		auto newcolumn = newperson.first;
		auto newline = newperson.second;

		{
			std::lock_guard lk(mt); // write-lock
			auto &oldcell = map.getCell(column, line);
			auto &newcell = map.getCell(newcolumn, newline);
			if (newcell) continue;
			map.move(oldcell, newcell);
			std::cout << "line: " << line << ", column: " << column << std::endl;
		}
		{
			std::lock_guard lk(mt);
			map.print();
		}

		line = newline;
		column = newcolumn;
	}
	{ // We're at the exit
		std::lock_guard lk(mt); // write-lock
		auto &oldcell = map.getCell(column, line);
		auto newcell = false;
		map.move(oldcell, newcell);
		std::cout << "Thread finished!" << std::endl;
	}
}

void init_threads(auto &threads, auto &map) {
	for (const auto &person: map.getPeople())
		threads.emplace_back(thread_main, std::ref(map), person.first, person.second);
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
