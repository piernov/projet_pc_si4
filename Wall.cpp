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

bool Wall::isReachable() {
	return false;
}

bool Wall::isLimit() {
	return false;
}
