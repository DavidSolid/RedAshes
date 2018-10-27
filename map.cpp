#include "map.h"
#include <map>
#include <string>
#include <chrono>
#include <cstdlib>

Map::Map(int l,int a,int b,const std::string& seed):RoomNumber(0),pixmap(new char[a*b]),x(a),y(b){
    for(int i=0;i<a*b;i++){
        pixmap[i]='0';
    }
    for(int i=0;i<l;i++){
        addRandomRoom(seed);
	}
    addPaths();
}

Map::~Map(){
    delete [] pixmap;
    for(Room* i:areas)
        delete i;
}

/*bool Map::IsInBounds(const Room& r) const{
	Room metaroom(0,0,x,y);
	return metaroom.isInArea(r.getVertex(0)) && metaroom.isInArea(r.getVertex(1)) && metaroom.isInArea(r.getVertex(2)) && metaroom.isInArea(r.getVertex(3));
}*/

bool Map::IsInBounds(const Coordinates& c) const{
    return c.getX()>=0 && c.getX()<x && c.getY()>=0 && c.getY()<y;
}

void Map::addRandomRoom(std::string code){
    std::seed_seq seed(code.begin(),code.end());
    std::default_random_engine enginex(seed);
    std::uniform_int_distribution<int> distributionx(0,x-7),distributiony(0,y-7);
	bool roomcreated=false;
    while(!roomcreated){
        auto seed1 = std::chrono::system_clock::now().time_since_epoch().count();
        std::default_random_engine enginey(seed1);
        int newx=distributionx(enginex);
        int newy=distributiony(enginey);
		bool vertexcol=false;
        for(unsigned int i=0;i<areas.size() && !vertexcol;i++){
            if(areas[i]->isInArea(Coordinates(newx,newy))){
				vertexcol=true;
			}
		}
		if(!vertexcol){
			int maxxl=x-newx;
			int maxyl=y-newy;
			bool validdim=false;
            while(!validdim && maxxl>=7 && maxyl>=7){
                std::uniform_int_distribution<int> distributionxl(7,maxxl),distributionyl(7,maxyl);
                int newxl=distributionxl(enginex);
                int newyl=distributionyl(enginey);
				Room newroom(newx,newy,newxl,newyl);
				validdim=true;
                for(unsigned int i=0;i<areas.size() && validdim;i++){
					if(areas[i]->isColliding(newroom)){
						validdim=false;
						maxyl--;
						maxxl--;
					}
                }
				if(validdim){
                    drawRoom(newroom);
					areas.push_back(new Room(newroom));
					roomcreated=true;
				}
			}
		}
	}
    RoomNumber++;
}
//areas
void Map::drawRoom(const Room& todraw){
    Coordinates v=todraw.getVertex();
    for(int i=0;i<todraw.getLengthX();i++){
        for(int j=0;j<todraw.getLengthY();j++){
            char toinsert= i==0 || j==0 || i==todraw.getLengthX()-1 || j==todraw.getLengthY()-1?'X':'-';
            pixmap[v.getX()+i+(v.getY()+j)*y]=toinsert;
        }
    }
}

void Map::addPaths(){
    std::vector<bool> trace(areas.size(),false);
    for(unsigned int i=0;i<areas.size();i++){
        Coordinates croom=areas[i]->getCenter();
        unsigned int minroom=i;
        int mindistance=INT_MAX;
        for(unsigned int j=0;j<areas.size();j++){
            if(i!=j && !trace[j]){
                int cdist=croom.distance(areas[j]->getCenter());
                if(cdist<mindistance){
                    minroom=j;
                    mindistance=cdist;
                }
            }
        }
        //routing algorithm
        trace[minroom]=true;
    }
}

std::vector<Coordinates> Map::pathfind(unsigned int start,unsigned int goal) const{
    std::vector<Coordinates> out;
    Coordinates cstart=areas[start]->getCenter();
    Coordinates cgoal=areas[goal]->getCenter();
    std::vector<Coordinates> checked;
    std::multimap<int,Coordinates>frontier;
    frontier.insert(std::make_pair(cstart.distance(cgoal),cstart));
    char* field=getPixmapExcluding(start,goal);
    Coordinates* track=new Coordinates[x*y];
    int* g=new int [x*y];
    for(int i=0;i<x*y;i++){
        track[i]={-1,-1};
        g[i]=INT_MAX;
    }
    g[x*cstart.getY()+cstart.getX()]=0;
    while(frontier.size()){
        Coordinates current=frontier.begin()->second;
        if(current==cgoal){
            out=buildpath(current,track);
            delete [] field; delete [] track; delete [] g;
            return out;
        }
        //
        checked.push_back(frontier.begin()->second);
        frontier.erase(frontier.cbegin());
        for(int i=0;i<4;i++){
            Coordinates toinsert=current.neighbour(i);
            if(IsInBounds(toinsert) && field[x*toinsert.getY()+toinsert.getX()]=='0'){
                bool inChecked=false;
                for(Coordinates i:checked){
                    if(i==toinsert){
                        inChecked=true;
                    }
                }
                if(!inChecked){
                    bool updateFrontier=false;
                    bool inFrontier=false;
                    int newg=g[x*current.getY()+current.getX()]+1;
                    std::multimap<int,Coordinates>::const_iterator posfront;
                    for(auto it=frontier.cbegin();it!=frontier.cend() && !inFrontier;it++){
                        if(it->second==toinsert){
                            inFrontier=true;
                            posfront=it;
                        }
                    }
                    if(inFrontier){
                        if(newg<g[x*toinsert.getY()+toinsert.getX()]){
                            frontier.erase(posfront);
                            updateFrontier=true;
                        }
                    }
                    else{
                        updateFrontier=true;
                    }
                    if(updateFrontier){
                        frontier.insert(std::make_pair(newg+toinsert.distance(cgoal),toinsert));
                        g[x*toinsert.getY()+toinsert.getX()]=newg;
                        track[x*toinsert.getY()+toinsert.getX()]=current;
                    }
                }
            }
        }
        //
    }

    delete [] field; delete [] track; delete [] g;
    return out;
}

std::vector<Coordinates> Map::buildpath(Coordinates current,Coordinates* trace) const{//tocheck
    std::vector<Coordinates> out;
    Coordinates ancestor=trace[x*current.getY()+current.getX()];
    if(ancestor!=Coordinates(-1,-1)){
        out=buildpath(ancestor,trace);
    }
    out.push_back(current);
    return out;
}

char* Map::getPixmapExcluding(unsigned int first,unsigned int last) const{
    char* npix=new char[x*y];
    for(int i=0;i<x*y;i++){
        npix[i]=pixmap[i];
    }
    Room* vfirst=areas[first];
    Room* vlast=areas[last];
    Room* erasing=vfirst;
    while(erasing){
        for(int i=0;i<erasing->getLengthY();i++)
            for(int j=0;j<erasing->getLengthX();i++)
                npix[erasing->getVertex().getX()+j+(erasing->getVertex().getY()+i)*x]='0';
        erasing=(erasing==vfirst)?vlast:nullptr;
    }
    return npix;
}

int Map::getDimX() const{return x;}

int Map::getDimY() const{return y;}

std::vector<Room*> Map::getAreas() const{return areas;}

std::ostream& operator<<(std::ostream& os,const Map& m){
    for(auto it:m.areas){
        os<<*it<<std::endl;
	}
    os<<std::endl;
    for(int i=0;i<m.y;i++){
        for(int j=0;j<m.x;j++){
            os<<m.pixmap[j+i*m.x]<<" ";
        }
        os<<std::endl;
    }
	return os;
}
