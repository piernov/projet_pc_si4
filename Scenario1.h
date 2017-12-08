#ifndef _SCENARIO1_H_
#define _SCENARIO1_H_

#include <array>

#include "Scenario.h"
#include "ConcurrentDeque.h"

class Scenario1: public Scenario {
private:
	std::array<ConcurrentDeque, 4> fifos = {};

	static void *thread_main(std::tuple<Map*, std::vector<Person*>, std::array<ConcurrentDeque, 4>*, int> *args); // needs to be static for pthread API
	void init_threads();
	void run_threads();
public:
	Scenario1(Map &map);
	~Scenario1() {};

	virtual void init() {};
	virtual void run();

};

#endif
