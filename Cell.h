#ifndef _CELL_H_
#define _CELL_H_

#include <condition_variable>
#include <mutex>

class Cell {
public:
	Cell();
	void arrive();
	void depart();
private:
	bool state = false;
	std::mutex mt;
	std::condition_variable cv;
};

#endif
