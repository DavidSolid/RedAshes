#ifndef COORDINATES_H
#define COORDINATES_H
class Coordinates{
private:
	int x,y;
public:
	Coordinates(int =0,int =0);
	int getX() const;
	int getY() const;
    Coordinates neighbour(int=0) const;
    int distance(const Coordinates&) const;
    bool operator==(const Coordinates&) const;
    bool operator!=(const Coordinates&) const;
};
#endif
