#ifndef RECT
#define RECT

class Rect{
private:
    short x; //x coordinate of top left corner
    short y; //y coordinate of top left corner
    short w;
    short h;
public:
    Rect();
    Rect(short xp, short yp, short wp, short hp); //p == 'prime' (not important but clarifies that there is a reason for the p

    void setX(short s); //for manual set of x0 if needed
    void setY(short s); //for manual set of y0 if needed
    void setW(short s); //for manual set of w if needed
    void setH(short s); //for manual set of h if needed

    short getX();
    short getY();
    short getW();
    short getH();

    void moveRight(short s);
    void moveLeft(short s);
    void moveUp(short s);
    void moveDown(short s);

    bool between(short s, short a, short b); //returns true if f is between a and b
    bool contains(Rect *r); //return true if any point in r is inside the calling Rect
    bool containsPoint(short x, short y); //returns true if coordinates are inside the calling Rect
};

#endif // RECT
