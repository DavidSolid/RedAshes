#ifndef MAP_H
#define MAP_H
#include <vector>
#include <iostream>
#include "room.h"
#include <random>
#include <string>

class Map{
	friend std::ostream& operator<<(std::ostream&,const Map&);
private:
	std::vector<Room*> areas;
	int RoomNumber;
    char* pixmap;
    int x,y;
public:
    Map(int, int, int, const std::string &);
    ~Map();
    bool IsInBounds(const Coordinates&) const;
	void addRandomRoom(std::string);
    void drawRoom(const Room&);
    void addPaths();
    std::vector<Coordinates> pathfind(unsigned int, unsigned int) const;
    std::vector<Coordinates> buildpath(Coordinates*) const;
    char* getPixmapExcluding(unsigned int, unsigned int) const;
    int getDimX() const;
    int getDimY() const;
    std::vector<Room*> getAreas() const;
};
std::ostream& operator<<(std::ostream&,const Map&);
#endif
