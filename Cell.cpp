#include "Cell.h"

Cell::Cell() {
}

void Cell::arrive() {
	{
		std::unique_lock<std::mutex> lk(mt);
		cv.wait(lk, [this]() { return !state; });

		state = true;

		lk.unlock();
		cv.notify_all();
	}
}

void Cell::depart() {
	{
		std::unique_lock<std::mutex> lk(mt);
		lk.lock();
		//cv.wait(lk, [this]() { return !state; });

		state = true;

		lk.unlock();
		cv.notify_all();
	}
}
