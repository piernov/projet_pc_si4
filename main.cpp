#include <iostream>
#include <vector>
#include <random>
#include <ctime>
#include <thread>
#include <mutex>
#include <iomanip>
#include <algorithm>

#include "Map.h"

static std::mutex mt;

static bool benchmark_mode = false;

static std::vector<double> cputimes;
static std::vector<double> walltimes;

void thread_main(Map &map, Person &person) {
	int column = person.getX();
	int line = person.getY();

	while (line != 0 || column > 2) { // algorithm only go north for now
		auto direction = map.computeDirection(column, line);

		direction = map.checkDirection(column, line, direction);

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
		}
		if (!benchmark_mode) {
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
	}
}

void init_threads(std::vector<std::thread> &threads, Map &map) {
	for (auto &person: map.getPeople()) {
		threads.emplace_back(thread_main, std::ref(map), std::ref(person));
	}
}

void run_threads(std::vector<std::thread> &threads, Map map) {
	std::clock_t c_start = std::clock();
	auto t_start = std::chrono::high_resolution_clock::now();

	init_threads(threads, map);
	for (auto &thread: threads)
		thread.join();

	threads.clear();

	std::clock_t c_end = std::clock();
	auto t_end = std::chrono::high_resolution_clock::now();

	auto cputime = 1000.0 * (c_end-c_start) / CLOCKS_PER_SEC;
	auto walltime = std::chrono::duration<double, std::milli>(t_end-t_start).count();

	cputimes.push_back(cputime);
	walltimes.push_back(walltime);
}

int main(int argc, char* argv[]) {
	// Map declaration
	Map map;

	// Command line parameters parsing
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
	if (!benchmark_mode)
		map.print();
	
	for (auto i = 0; i < 5; i++) {
		run_threads(threads, map);
	}


	/*if (benchmark_mode) {
		std::cout << std::fixed << std::setprecision(2) << "CPU time used: "
        	      << 1000.0 * (c_end-c_start) / CLOCKS_PER_SEC << " ms\n"
		      << "Wall clock time passed: "
        	      << std::chrono::duration<double, std::milli>(t_end-t_start).count()
        	      << " ms" << std::endl;
	}*/

	if (benchmark_mode) {
		std::sort(walltimes.begin(), walltimes.end());
		std::sort(cputimes.begin(), cputimes.end());

		auto walltimemean = (walltimes[1] + walltimes[2] + walltimes[3]) / 3;
		auto cputimemean = (cputimes[1] + cputimes[2] + cputimes[3]) / 3;


		std::cout << std::fixed << std::setprecision(2) << "CPU time used: " << cputimemean << " ms" << std::endl;
		std::cout << "Wall clock time passed: " << walltimemean << " ms" << std::endl;
	}

	return 0;
}
