#include "Scenario1.h"

#include <iostream>
#include <tuple>
#include <algorithm>
#include <ctime>
#include <chrono>

int Scenario1::people_remaining = 0;
pthread_mutex_t Scenario1::counter_mutex;

int Scenario1::getcounter() {
	pthread_mutex_lock(&counter_mutex);
	int tmp = Scenario1::people_remaining;
	pthread_mutex_unlock(&counter_mutex);
	return tmp;
}

void Scenario1::decrcounter() {
	pthread_mutex_lock(&counter_mutex);
	Scenario1::people_remaining--;
	pthread_mutex_unlock(&counter_mutex);
}

Scenario1::Scenario1(Map &map) : Scenario(map) {
	threads.reserve(4);
	people_remaining = map.getPeopleCount();

	if(pthread_mutex_init(&counter_mutex, NULL) != 0) {
		std::cerr << "pthread_mutex_init" << std::endl;
		exit(1);
	}
}

Scenario1::~Scenario1() {
	pthread_mutex_destroy(&counter_mutex);
}

void *Scenario1::thread_main(std::tuple<Map*, std::vector<Person*>, std::array<ConcurrentDeque*, 4>*, int> *args) {
	if (args == nullptr) {
		std::cerr << "thread_main args is null" << std::endl;
		exit(1);
	}
	if (std::get<0>(*args) == nullptr) {
		std::cerr << "thread_main map is null" << std::endl;
		exit(1);
	}
	auto map = *std::get<0>(*args); // hides class-scope map
	auto people = std::get<1>(*args);
	auto fifos = std::get<2>(*args);
	auto this_tid = std::get<3>(*args);
	auto fifo = fifos->at(this_tid);

	while (getcounter()) {
		while (!fifo->isEmpty()) {
			Person *p = fifo->pop_front();
			people.push_back(p);
		}

		auto i = people.begin();
		while (i != people.end()) { // Not using range-based for loop since we're removing elements
			auto person = *i;
			int column = person->getX();
			int line = person->getY();

			if (line == -1 ||column == -1) // end condition for one person
				continue;//TODO: useless since person is deleted from list

			if (((line == 0 && column == 0) || (line == 0 && column == 1)|| (line == 1 && column == 0))) { //TODO: remove person 
				Space* oldcell = map.getCell(column, line);
				if (oldcell != nullptr)
					oldcell->depart();
				person->setX(-1);
				person->setY(-1);

				decrcounter();
				people.erase(i);
				continue;
			}

			auto newperson = map.getNextPosition(column, line);
			auto newcolumn = newperson.first;
			auto newline = newperson.second;

			{
				Space* oldcell = map.getCell(column, line);
				Space* newcell = map.getCell(newcolumn, newline);
				if (newcell != nullptr && !newcell->isReachable()) {
					i++;
					continue;
				}
				if (newcell != nullptr)
					newcell->arrive();
				if (oldcell != nullptr)
					oldcell->depart();
				person->setX(newcolumn);
				person->setY(newline);

				if (newcell->isLimit()) { // v2
					auto newpos = map.getNextPosition(newcolumn, newline);
					auto tid = map.getTID(newpos);
					auto nextcell = map.getCell(newpos.first, newpos.second);
					if (nextcell != nullptr && nextcell->isLimit()) { // check if we cross the limit (this_tid != tid)
						/*nextcell->arrive();
						newcell->depart();
						person->setX(newpos.first);
						person->setY(newpos.second);*/
						fifos->at(tid)->push_back(person);

						people.erase(i);
						continue; // removing an element so we don't need to go to the next position since all following elements are shifted
					}
					// need to delete person from this thread's management
				}
			}
			if (!benchmark_mode) {
				printMap(map);
			}

			i++;
		}
	}

	delete args;
	args = nullptr;
	return nullptr;
}

void Scenario1::init_threads() {
	for (auto i = 0; i < 4; i++) {
		pthread_t thread;
		auto args = new std::tuple<Map*, std::vector<Person*>, std::array<ConcurrentDeque*, 4>*, int>(&map, map.getPeople(i), &fifos, i); // map, people for this thread, fifos, thread id
		if (pthread_create(&thread, NULL, reinterpret_cast<void *(*)(void *)>(thread_main), args)) {
			std::cerr << "pthread_create" << std::endl;
			exit(1);
		}
		threads.push_back(std::move(thread));
	}
}

void Scenario1::run_threads() {
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

void Scenario1::run() {
	run_threads();
}
