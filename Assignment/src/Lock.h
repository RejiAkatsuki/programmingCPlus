class Lock {
public :
    int lX,lY; // position on map
    int lCh; // ascii code
    TCODColor lCol; // color
 
    Lock(int x, int y, int ch, const TCODColor &col);
    void render() const;
};
