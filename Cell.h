#ifndef _CELL_H_
#define _CELL_H_


#include <pthread.h>

#include <ostream>
#include "Space.h"

class Cell : public Space {
public:
	Cell();
	~Cell();

	void arrive() override;

	void depart() override;

	bool isWall() override;

	std::ostream &operator<<(std::ostream &os);
	std::ostream &print(std::ostream &os) override ;

private:
	bool state = false;
	pthread_mutex_t mutex;
	pthread_cond_t condition;
};

#endif
