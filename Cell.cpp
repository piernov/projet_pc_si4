#include "Cell.h"

Cell::Cell() {
}

void Cell::arrive() {
	state = true;
}

void Cell::depart() {
	state = false;
}
