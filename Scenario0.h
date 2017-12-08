#ifndef _SCENARIO0_H_
#define _SCENARIO0_H_

#include <array>

#include "Scenario.h"

class Scenario0: public Scenario {
private:

public:
	Scenario0(Map &map) : Scenario(map) {}

	virtual void init();
	virtual void run();
};

#endif
