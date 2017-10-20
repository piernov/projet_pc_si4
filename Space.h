//
// Created by Florian Bourniquel on 20/10/2017.
//

#ifndef PROJET_PC_SI4_SPACE_H
#define PROJET_PC_SI4_SPACE_H


#include <ostream>

class Space {

public:
	virtual bool isWall() = 0;
	virtual void arrive() = 0;

	virtual void depart() = 0;
	virtual std::ostream &print(std::ostream &os) = 0;
};


#endif //PROJET_PC_SI4_SPACE_H
