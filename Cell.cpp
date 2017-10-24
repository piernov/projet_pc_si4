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

Cell::~Cell() {
	pthread_cond_destroy(&condition);
	pthread_mutex_destroy(&mutex);
}

void Cell::arrive() {
	{
		pthread_mutex_lock(&mutex);
		while (state) pthread_cond_wait(&condition, &mutex);

		state = true;

		pthread_mutex_unlock(&mutex);
		pthread_cond_broadcast(&condition);
	}
}

void Cell::depart() {
	{
		pthread_mutex_lock(&mutex);

		state = false;

		pthread_mutex_unlock(&mutex);
		pthread_cond_broadcast(&condition);
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

