//
// Created by Florian Bourniquel on 01/12/2017.
//

#ifndef PROJET_PC_SI4_CONCURRENTDEQUE_H
#define PROJET_PC_SI4_CONCURRENTDEQUE_H


#include "Person.h"
#include <pthread.h>
#include <deque>

using std::deque;

class ConcurrentDeque {
public:
	ConcurrentDeque();

	Person* pop_front();
 	void push_back(Person* person);
	bool isEmpty();
	int size() {
		return m_deque.size();
	
	}
private:
	pthread_mutex_t mutex;
	deque<Person*> m_deque = {};
};


#endif //PROJET_PC_SI4_CONCURRENTDEQUE_H
