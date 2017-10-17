#ifndef _PERSON_H_
#define _PERSON_H_

class Person {
public:
	Person(int x, int y);

	int getX() const { return x; }
	int getY() const { return y; }

	void setX(int x) { this->x = x; }
	void setY(int y) { this->y = y; }

	bool operator ==(const Person &p) const;
	bool operator !=(const Person &p) const;
private:
	int x;
	int y;
};

#endif
