#include <vector>
using namespace std;

class Guard {
public :
    int gX,gY; // position on map
    int gCh; // ascii code
    TCODColor gCol; // color
 
    Guard(int x, int y, int ch, const TCODColor &col);
    void render() const;
    void moveUp(int mapWidth, int mapHeight, vector<char>element);
    void moveDown(int mapWidth, int mapHeight, vector<char>element);
    void moveLeft(int mapWidth, int mapHeight, vector<char>element);
    void moveRight(int mapWidth, int mapHeight, vector<char>element);
};
    
