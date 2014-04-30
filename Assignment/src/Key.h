class Key {
public :
    int kX,kY; // position on map
    int kCh; // ascii code
    TCODColor kCol; // color
 
    Key(int x, int y, int ch, const TCODColor &col);
    void render() const;
};
