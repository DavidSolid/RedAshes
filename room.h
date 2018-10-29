#include "coordinates.h"
#include <iostream>
class Room{
private:
	Coordinates vertex; //bottom left vertex
	int xlength,ylength;
public:
	Room(int,int,int,int);
	Coordinates getVertex(int =0) const; //0 to 3 counter-clockwise vertex
    Coordinates getCenter() const;
	int getLengthX() const;
    int getLengthY() const;
	bool isInArea(const Coordinates&) const;
    bool isBorder(const Coordinates&) const;
	bool isColliding(const Room&) const;
};
std::ostream& operator<<(std::ostream&,const Room&);
