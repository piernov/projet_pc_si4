#ifndef _SCENARIO_H_
#define _SCENARIO_H_

#include <vector>
#include <iostream>
#include "pthread.h"

#include "Map.h"

// should be used like a singleton: only one instance at a time since it unfortunatel relies on static members
// due to pthread interface not accepting member function for new threads
class Scenario {
protected:
	Map &map;

	std::vector<pthread_t> threads;

	static pthread_mutex_t mt;

	double cputime = 0.0;
	double walltime = 0.0;

	static bool benchmark_mode;
public:
	Scenario(Map &map) : map(map) {
		// Init display mutex
		if(pthread_mutex_init(&mt, NULL) != 0) {
			std::cerr << "pthread_mutex_init" << std::endl;
			exit(1);
		}

	}
	virtual ~Scenario() {
		// Destroy display mutex
		pthread_mutex_destroy(&mt);
	}

	virtual void run() = 0;

	void setBenchmarkMode(bool m) {
		benchmark_mode = m;
	}

	static void printMap(Map &map) { // static because called from thread_main
		pthread_mutex_lock(&mt);
		map.print();
		pthread_mutex_unlock(&mt);
	}

	double getCPUTime() const {
		return cputime;
	}

	double getWallTime() const {
		return walltime;
	}
};
#endif
