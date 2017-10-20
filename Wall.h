//
// Created by Florian Bourniquel on 20/10/2017.
//

#ifndef PROJET_PC_SI4_WALL_H
#define PROJET_PC_SI4_WALL_H


#include <ostream>
#include "Space.h"

class Wall : public Space {
public:
	bool isWall() override;

	std::ostream &operator<<(std::ostream &os);
	std::ostream &print(std::ostream &os) override ;

};


#endif //PROJET_PC_SI4_WALL_H
