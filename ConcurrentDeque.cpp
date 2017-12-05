//
// Created by Florian Bourniquel on 01/12/2017.
//

#include <iostream>
#include "ConcurrentDeque.h"

ConcurrentDeque::ConcurrentDeque() {
	if(pthread_mutex_init(&mutex, NULL) != 0) {
		std::cerr << "pthread_mutex_init" << std::endl;
		exit(1);
	}
}

Person * ConcurrentDeque::pop_front() {
	pthread_mutex_lock(&mutex);
	Person* person = m_deque.front();
	m_deque.pop_front();
	pthread_mutex_unlock(&mutex);
	return person;
}

void ConcurrentDeque::push_back(Person *person) {
	pthread_mutex_lock(&mutex);
	m_deque.push_back(person);
	pthread_mutex_unlock(&mutex);
}

bool ConcurrentDeque::isEmpty() {
	pthread_mutex_lock(&mutex);
	bool tmp = m_deque.empty();
	pthread_mutex_unlock(&mutex);
	return tmp;
}
