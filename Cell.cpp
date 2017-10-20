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
		//cv.wait(lk, [this]() { return !state; });

		state = false;

		lk.unlock();
		cv.notify_all();
	}
}

bool Cell::isWall() {
	return false;
}

std::ostream &Cell::operator<<(std::ostream &os) {
	if (state)
		os << "P";
	else
		os << "C";
	return os;
}

std::ostream &Cell::print(std::ostream &os) {
	if (state)
		os << "x";
	else
		os << ".";
	return os;
}

