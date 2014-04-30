class Money {
public :
    int mX,mY; // position on map
    int mCh; // ascii code
    TCODColor mCol; // color
 
    Money(int x, int y, int ch, const TCODColor &col);
    void render() const;
};
