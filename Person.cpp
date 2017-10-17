#include "Person.h"


Person::Person(int x, int y): x(x), y(y) {
}


bool Person::operator ==(const Person &p) const {
	return x == p.getX() && y == p.getY();
}

bool Person::operator !=(const Person &p) const {
	return !(p == *this);
}
