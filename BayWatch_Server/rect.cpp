#include "rect.h"

Rect::Rect():x(0.0), y(0.0), w(100.0), h(100.0){};
Rect::Rect(short xp, short yp, short wp, short hp):x(xp), y(yp), w(wp), h(hp){};

void Rect::setX(short xp){
    x = xp;
};

void Rect::setY(short yp){
    y = yp;
};

void Rect::setW(short wp){
    w = wp;
};

void Rect::setH(short hp){
    h = hp;
};

short Rect::getX(){
    return x;
};

short Rect::getY(){
    return y;
};

short Rect::getW(){
    return w;
};

short Rect::getH(){
    return h;
};

void Rect::moveRight(short s){
    x += s;
};

void Rect::moveLeft(short s){
    x -= s;
};

void Rect::moveUp(short s){
    y -= s;
};

void Rect::moveDown(short s){
    y += s;
};

bool Rect::between(short s, short a, short b){
    return ((a <= s)&&(s <= b))?true:false;
};

bool Rect::contains(Rect *r){
    //concept: if any corner of r is in this Rect, the two Rect objects touch (this Rect contains r)

    //(X comps)
    //if r.x is between this.x and this.x + this.w
    //OR
    //if r.x + r.x is between this.x and this.x + this.w

    //AND

    //(Y comps)
    //if r.y is between this.y and this.y + this.h
    //OR
    //if r.y + r.y is between this.y and this.y + this.y

    //THEN return true, ELSE return false
    if(between(r->getX(), x, x+w) || between(r->getX()+r->getW(), x, x+w)){
        if(between(r->getY(), y, y+h) || between(r->getY()+r->getH(), y, y+h)){
            return true;
        }
    }

    return false;
};

bool Rect::containsPoint(short x2, short y2){
    if(between(x2, x, x+w) && between(y2, y, y+h)){
        return true;
    }

    return false;
};
