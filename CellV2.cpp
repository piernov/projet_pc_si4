//
// Created by Florian Bourniquel on 01/12/2017.
//

#include "CellV2.h"


void CellV2::arrive() {
	state = true;
}

void CellV2::depart() {
	state = false;
}

bool CellV2::isWall() {
	return false;
}

std::ostream &CellV2::operator<<(std::ostream &os) {
	if (state)
		os << "P";
	else
		os << "C";
	return os;
}

std::ostream &CellV2::print(std::ostream &os) {
	if (state)
		os << "x";
	else
		os << ".";
	return os;
}

bool CellV2::isReachable() {
	return !state;
}

bool CellV2::isLimit() {
	return limit;
}

