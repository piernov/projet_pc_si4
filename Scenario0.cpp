#include "Scenario0.h"

#include <iostream>
#include <tuple>
#include <algorithm>
#include <ctime>
#include <chrono>

Scenario0::Scenario0(Map &map) : Scenario(map) {
	threads.reserve(map.getPeopleCount());
}

Scenario0::~Scenario0() {
}

void *Scenario0::thread_main(std::pair<Map*, Person*> *args) {
	auto map = *(args->first);
	auto person = *(args->second);

	int column = person.getX();
	int line = person.getY();

	while (!((line == 0 && column == 0) || (line == 0 && column == 1)|| (line == 1 && column == 0))) { // algorithm only go north for now
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
			printMap(map);
		}


	}
	{ // We're at the exit
		Space* oldcell = map.getCell(column, line);
		if (oldcell != nullptr)
			oldcell->depart();
		person.setX(0);
		person.setY(0);
	}

	delete args;
	args = nullptr;
	return nullptr;
}


void Scenario0::init_threads() {
	for (auto &person: map.getPeople()) {
		pthread_t thread;
		auto args = new std::pair<Map*, Person*>(&map, person);
		if (pthread_create(&thread, NULL, reinterpret_cast<void *(*)(void *)>(thread_main), args)) {
			std::cerr << "pthread_create" << std::endl;
			exit(1);
		}
		threads.push_back(std::move(thread));
	}
}

void Scenario0::run_threads() {
	std::clock_t c_start = std::clock();
	auto t_start = std::chrono::high_resolution_clock::now();

	init_threads();
	for (auto &thread: threads)
		pthread_join(thread, NULL);

	threads.clear();

	std::clock_t c_end = std::clock();
	auto t_end = std::chrono::high_resolution_clock::now();

	cputime = 1000.0 * (c_end-c_start) / CLOCKS_PER_SEC;
	walltime = std::chrono::duration<double, std::milli>(t_end-t_start).count();
}

void Scenario0::run() {
	run_threads();
}
