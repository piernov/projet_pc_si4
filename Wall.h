#ifndef PROJET_PC_SI4_WALL_H
#define PROJET_PC_SI4_WALL_H


#include <ostream>
#include "Space.h"

class Wall : public Space {
public:
	bool isWall() override;


	void arrive() override;

	void depart() override;
	std::ostream &operator<<(std::ostream &os);
	std::ostream &print(std::ostream &os) override ;
	bool isReachable() override;

	bool isLimit() override;

};


#endif //PROJET_PC_SI4_WALL_H
