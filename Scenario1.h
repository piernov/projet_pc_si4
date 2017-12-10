#ifndef _SCENARIO1_H_
#define _SCENARIO1_H_

#include <array>

#include "Scenario.h"
#include "ConcurrentDeque.h"

// should be used like a singleton: only one instance at a time since it unfortunatel relies on static members
// due to pthread interface not accepting member function for new threads
class Scenario1: public Scenario {
private:
	std::array<ConcurrentDeque*, 4> fifos = {new ConcurrentDeque(), new ConcurrentDeque(), new ConcurrentDeque(), new ConcurrentDeque()};

	static void *thread_main(std::tuple<Map*, std::vector<Person*>, std::array<ConcurrentDeque*, 4>*, int> *args); // needs to be static for pthread API
	void init_threads();
	void run_threads();

	static void decrcounter(); // locked counter interface, static because used by thread_main
	static int getcounter(); // locked counter interface, static because used by thread_main

	static int people_remaining; // static because used by thread_main
	static pthread_mutex_t counter_mutex; // static because used by thread_main

public:
	Scenario1(Map &map);
	~Scenario1();

	virtual void run();
};

#endif
