#include <string>
#include <vector>
#include "rect.h"
#include "ticket.h"
#include "box.h"

Box::Box():label("Bay"), tickets(), boundary(), active(false), selected(false), status(selectStatuses::standby){};

Box::Box(short x, short y):label("Bay"), tickets(), boundary(x, y, 200, 400), active(false), selected(false),
        status(selectStatuses::standby){};

Box::Box(short x, short y, short w, short h):
        label("Bay"), tickets(), boundary(x, y, w, h), active(false), selected(false), status(selectStatuses::standby){};

Box::Box(std::string newL):label(newL), tickets(), boundary(), active(false), selected(false),
        status(selectStatuses::standby){};

Box::Box(std::string newL, short x, short y):label(newL), tickets(), boundary(x, y, 200, 400), active(false), selected(false),
        status(selectStatuses::standby){};

Box::Box(std::string newL, short x, short y, short w, short h):
        label(newL), tickets(), boundary(x, y, w, h), active(false), selected(false), status(selectStatuses::standby){};

void Box::setLabel(std::string s){
    label = s;
};

void Box::setActive(bool b){
    active = b;
};

void Box::setSelected(bool b){
    selected = b;

    //if selected is false, set select status to standby; else, update status accordingly
    if(selected == false){
        status = selectStatuses::standby;
    }else{
        if(status == selectStatuses::standby || status == selectStatuses::stage3){
            status = selectStatuses::stage1;
        }else
        if(status == selectStatuses::stage1){
            status = selectStatuses::stage2;
        }else
        if(status == selectStatuses::stage2){
            status = selectStatuses::stage3;
        }
    }
};

void Box::setStatus(unsigned short s){
    status = s;
};

unsigned short Box::getStatus(){
    return status;
};

std::vector<Ticket*> Box::getTickets(){
    return tickets;
};

Rect* Box::getBoundary(){
    return &boundary;
};

std::string Box::getLabel(){
    return label;
};

bool Box::isActive(){
    return active;
};

bool Box::isSelected(){
    return selected;
};

void Box::addTicket(std::string newID, std::string newV, std::string newN, short s){
    //Ticket newT(newID, newV, newN, s);
    tickets.push_back(new Ticket(newID, newV, newN));
};

//takes string 'id' as a parameter
//if the id parameter matches the id of any ticket
//the first ticket instance that matched is removed
//if no match is found, nothing happens
void Box::removeTicket(std::string id){ //UNUSED: ticket object changed to tickets, method no longer usable
    for(unsigned short i = 0; i < tickets.size(); i++){
        if(id.compare(tickets[i]->getID()) == 0){
            tickets.erase(tickets.begin()+i);
            break;
        }
    }
};
