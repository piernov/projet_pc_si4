#include <iostream>

#include "Cell.h"

Cell::Cell() {
	if(pthread_mutex_init(&mutex, NULL) != 0) {
		std::cerr << "pthread_mutex_init" << std::endl;
		exit(1);
	}
	if(pthread_cond_init(&condition, NULL) != 0) {
		std::cerr << "pthread_cond_init" << std::endl;
		exit(1);
	}
}

void Cell::arrive() {
	{
//		std::unique_lock<std::mutex> lk(mt);
		pthread_mutex_lock(&mutex);
		while (state) pthread_cond_wait(&condition, &mutex);
		//cv.wait(lk, [this]() { return !state; });

		state = true;

		pthread_mutex_unlock(&mutex);
		pthread_cond_broadcast(&condition);

		//lk.unlock();
		//cv.notify_all();
	}
}

void Cell::depart() {
	{
		//std::unique_lock<std::mutex> lk(mt);
		pthread_mutex_lock(&mutex);
		//cv.wait(lk, [this]() { return !state; });

		state = false;

		pthread_mutex_unlock(&mutex);
		pthread_cond_broadcast(&condition);
		//lk.unlock();
		//cv.notify_all();
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

