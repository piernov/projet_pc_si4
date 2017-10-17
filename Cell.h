#ifndef _CELL_H_
#define _CELL_H_

class Cell {
public:
	Cell();
	void arrive();
	void depart();
private:
	bool state = false;
};

#endif
