#include "room.h"
#include <cmath>

Room::Room(int tx,int ty,int x,int y):vertex(tx,ty),xlength(x),ylength(y){}

Coordinates Room::getVertex(int v) const{
    Coordinates out;
	switch(v){
		case 0:
            out=vertex;
        break;
		case 1:
            out=Coordinates(vertex.getX()+getLengthX()-1,vertex.getY());
        break;
		case 2:
            out=Coordinates(vertex.getX()+getLengthX()-1,vertex.getY()+getLengthY()-1);
        break;
		case 3:
            out=Coordinates(vertex.getX(),vertex.getY()+getLengthY()-1);
        break;
		default:
            out=Coordinates();
	}
    return out;
}

Coordinates Room::getCenter() const{
    return {getVertex().getX()+static_cast<int>(std::ceil(getLengthX()/2)),getVertex().getY()+static_cast<int>(std::ceil(getLengthY()/2))};
}

bool Room::isInArea(const Coordinates& c) const{
    return c.getX()>=vertex.getX() && c.getX()<=vertex.getX()+getLengthX()-1 && c.getY()>=vertex.getY() && c.getY()<=vertex.getY()+getLengthY()-1;
}

bool Room::isBorder(const Coordinates& c) const{
    return ((c.getX()==getVertex().getX() || c.getX()==getVertex(1).getX()) && c.getY()>=vertex.getY() && c.getY()<=getVertex(3).getY()) ||
            ((c.getY()==getVertex().getY() || c.getY()==getVertex(3).getY()) && c.getX()>=vertex.getX() && c.getX()<=getVertex(1).getX());
}

bool Room::isColliding(const Room& r) const{
    return !((getVertex().getX()>r.getVertex().getX() && getVertex().getX()>r.getVertex(1).getX() && getVertex().getX()>r.getVertex(2).getX() && getVertex().getX()>r.getVertex(3).getX()) ||
            (getVertex().getY()>r.getVertex().getY() && getVertex().getY()>r.getVertex(1).getY() && getVertex().getY()>r.getVertex(2).getY() && getVertex().getY()>r.getVertex(3).getY()) ||
            (getVertex(2).getX()<r.getVertex().getX() && getVertex(2).getX()<r.getVertex(1).getX() && getVertex(2).getX()<r.getVertex(2).getX() && getVertex(2).getX()<r.getVertex(3).getX()) ||
            (getVertex(2).getY()<r.getVertex().getY() && getVertex(2).getY()<r.getVertex(1).getY() && getVertex(2).getY()<r.getVertex(2).getY() && getVertex(2).getY()<r.getVertex(3).getY()));
}

int Room::getLengthX() const{return xlength;}

int Room::getLengthY() const{return ylength;}

std::ostream& operator<<(std::ostream& os,const Room& r){
    return os<<"Coordinate(x,y): "<<r.getVertex().getX()<<"-"<<r.getVertex().getY()<<" Dimensions(x,y): "<<r.getLengthX()<<"-"<<r.getLengthY();
}
