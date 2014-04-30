#include <vector>
#include <iostream>
using namespace std;

struct Tile {
    bool canWalk; // can we walk through this tile?
    Tile() : canWalk(true) {}
};
 
class Map {
public :
    int width, height;
    vector <char> element;
    Map(int, int, vector<char>);
    ~Map();
    bool isWall(int x, int y) const;//check if can walk through
    void render() const;//display map
protected :
    Tile *tiles;
    void setWall(int x, int y);
};


