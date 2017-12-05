#include <iostream>
#include <vector>
#include <random>
#include <ctime>
#include <chrono>
#include <pthread.h>
#include <iomanip>
#include <algorithm>

#include "Map.h"


static pthread_mutex_t mt;

static int person_count = 0;
static bool benchmark_mode = false;
static int threads_mode = 0;

static int people_not_finished = 1 << person_count;

static std::vector<double> cputimes;
static std::vector<double> walltimes;

ConcurrentDeque fifos[4] = {};

void addToFIFO(int tid, Person *p) {
	fifos[tid].push_back(person);
}

void *thread_main(std::tuple<Map*, std::vector<Person*>, ConcurrentDeque*> *args) {
	auto map = *(std::get<0>(args));
	auto people = *(std::get<1>(args));
	auto fifo = *(std::get<2>(args));


	int column = person.getX();
	int line = person.getY();

	while (people_not_finished) {
		while (!fifo.isEmpty()) {
			Person *p = fifo.pop_front();
			people.push_back(p);
		}

		for (auto &person: people) {
			int column = person.getX();
			int line = person.getY();

			if (line == -1 ||column == -1) // end condition for one person
				continue;

			if (((line == 0 && column == 0) || (line == 0 && column == 1)|| (line == 1 && column == 0))) { //TODO: remove person 
				Space* oldcell = map.getCell(column, line);
				if (oldcell != nullptr)
					oldcell->depart();
				person.setX(-1);
				person.setY(-1);
				people_not_finished--;
			}

			auto newperson = map.getNextPosition(column, line);
			auto newcolumn = newperson.first;
			auto newline = newperson.second;

			{
				Space* oldcell = map.getCell(column, line);
				Space* newcell = map.getCell(newcolumn, newline);
				if (threads_mode == 1 && !newcell.isReachable()) // v2
					continue;
				if (newcell != nullptr)
					newcell->arrive();
				if (oldcell != nullptr)
					oldcell->depart();
				person.setX(newcolumn);
				person.setY(newline);

				line = person.getY();
				column = person.getX();

				if (threads_mode == 1 && newcell.isLimit()) { // v2
					auto newperson = map.getNextPosition(column, line);
					auto tid = map.getTID(newperson);
					addToFIFO(tid, newperson);
				}
			}
			if (!benchmark_mode) {
				pthread_mutex_lock(&mt);
				map.print();
				pthread_mutex_unlock(&mt);
			}


		}
	}

	delete args;
	args = nullptr;
	return nullptr;
}

void init_threads(std::vector<pthread_t> &threads, Map &map) {
	for (auto i = 0; i < 4; i++) {
	//for (auto &person: map.getPeople()) {
		pthread_t thread;
		auto args = new std::tuple<Map*, std::vector<Person*>, ConcurrentDeque*>(&map, &person, &fifos[i]);
		if (pthread_create(&thread, NULL, reinterpret_cast<void *(*)(void *)>(thread_main), args)) {
			std::cerr << "pthread_create" << std::endl;
			exit(1);
		}
		threads.push_back(std::move(thread));
	}
}

void run_threads(std::vector<pthread_t> &threads, Map map) {
	std::clock_t c_start = std::clock();
	auto t_start = std::chrono::high_resolution_clock::now();

	init_threads(threads, map);
	for (auto &thread: threads)
		pthread_join(thread, NULL);

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

	// Init display mutex
	if(pthread_mutex_init(&mt, NULL) != 0) {
		std::cerr << "pthread_mutex_init" << std::endl;
		exit(1);
	}

	// Command line parameters parsing
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
	std::vector<pthread_t> threads;
	threads.reserve(4);


	map.init(person_count);
	if (!benchmark_mode)
		map.print();
	
	for (auto i = 0; i < 5; i++) {
		run_threads(threads, map);
	}

	if (benchmark_mode) {
		std::sort(walltimes.begin(), walltimes.end());
		std::sort(cputimes.begin(), cputimes.end());

		auto walltimemean = (walltimes[1] + walltimes[2] + walltimes[3]) / 3;
		auto cputimemean = (cputimes[1] + cputimes[2] + cputimes[3]) / 3;


		std::cout << std::fixed << std::setprecision(2) << "CPU time used: " << cputimemean << " ms" << std::endl;
		std::cout << "Wall clock time passed: " << walltimemean << " ms" << std::endl;
	}

	// Destroy display mutex
	pthread_mutex_destroy(&mt);

	return 0;
}
