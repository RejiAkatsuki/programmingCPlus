class Actor {
public :
    int aX,aY; // position on map
    int aCh; // ascii code
    TCODColor aCol; // color
 
    Actor(int x, int y, int ch, const TCODColor &col);
    void render() const;
};
