#ifndef _SCENARIO0_H_
#define _SCENARIO0_H_

#include <array>

#include "Scenario.h"

// should be used like a singleton: only one instance at a time since it unfortunatel relies on static members
// due to pthread interface not accepting member function for new threads
class Scenario0: public Scenario {
private:
	static void *thread_main(std::pair<Map*, Person*> *args);
	void init_threads();
	void run_threads();
public:
	Scenario0(Map &map);
	~Scenario0();

	virtual void run();
};

#endif
