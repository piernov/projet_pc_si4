//
// Created by Florian Bourniquel on 01/12/2017.
//

#ifndef PROJET_PC_SI4_CELLV2_H
#define PROJET_PC_SI4_CELLV2_H


#include "Space.h"

class CellV2 : public Space {
public:
	CellV2(bool limit) : limit(limit) {};
	~CellV2() = default;

	void arrive() override;

	void depart() override;

	bool isWall() override;

	std::ostream &operator<<(std::ostream &os);
	std::ostream &print(std::ostream &os) override ;
	bool isReachable() override;

	bool isLimit() override;

private:
	bool state = false;
	bool limit;
};


#endif //PROJET_PC_SI4_CELLV2_H
