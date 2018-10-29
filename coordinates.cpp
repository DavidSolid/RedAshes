#include "coordinates.h"
#include <cmath>

Coordinates::Coordinates(int a,int b):x(a),y(b){}

int Coordinates::getX() const{return x;}

int Coordinates::getY() const{return y;}

int Coordinates::distance(const Coordinates& c) const{
    return static_cast<int>(std::abs(getX()-c.getX())+std::abs(getY()-c.getY()));
}

bool Coordinates::operator==(const Coordinates& c) const{
    return x==c.x && y==c.y;
}

bool Coordinates::operator!=(const Coordinates& c) const{
    return x!=c.x || y!=c.y;
}

Coordinates Coordinates::neighbour(int p) const{
    Coordinates out;
    switch(p){
    case 0:
        out=Coordinates(getX()+1,getY());
        break;
    case 1:
        out=Coordinates(getX(),getY()+1);
        break;
    case 2:
        out=Coordinates(getX()-1,getY());
        break;
    case 3:
        out=Coordinates(getX(),getY()-1);
        break;
    }
    return out;
}
