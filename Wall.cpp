//
// Created by Florian Bourniquel on 20/10/2017.
//

#include "Wall.h"

bool Wall::isWall() {
	return true;
}

std::ostream & Wall::operator<<(std::ostream &os) {
	os << "1";
	return os;
}

std::ostream &Wall::print(std::ostream &os) {
	os << "1";
	return os;
}

void Wall::arrive() {

}

void Wall::depart() {

}
