#include <string>
#include <sstream>
#include <vector>
#include <chrono>
#include "rect.h"
#include "ticket.h"
#include "box.h"

Box::Box():label("Bay"), tickets(), boundary(), active(false), selected(false), status(selectStatuses::standby),
        activeStartPoint(std::chrono::steady_clock::now()){};

Box::Box(short x, short y):label("Bay"), tickets(), boundary(x, y, 200, 400), active(false), selected(false),
        status(selectStatuses::standby), activeStartPoint(std::chrono::steady_clock::now()){};

Box::Box(short x, short y, short w, short h):
        label("Bay"), tickets(), boundary(x, y, w, h), active(false), selected(false), status(selectStatuses::standby),
        activeStartPoint(std::chrono::steady_clock::now()){};

Box::Box(std::string newL):label(newL), tickets(), boundary(), active(false), selected(false),
        status(selectStatuses::standby), activeStartPoint(std::chrono::steady_clock::now()){};

Box::Box(std::string newL, short x, short y):label(newL), tickets(), boundary(x, y, 200, 400), active(false), selected(false),
        status(selectStatuses::standby), activeStartPoint(std::chrono::steady_clock::now()){};

Box::Box(std::string newL, short x, short y, short w, short h):
        label(newL), tickets(), boundary(x, y, w, h), active(false), selected(false), status(selectStatuses::standby),
        activeStartPoint(std::chrono::steady_clock::now()){};

void Box::setLabel(std::string s){
    label = s;
};

void Box::setActive(bool b){
    active = b;
    if(active == true){
        setActiveStartPoint(std::chrono::steady_clock::now());
    }
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

void Box::setActiveStartPoint(std::chrono::steady_clock::time_point p){
    activeStartPoint = p;
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

unsigned short Box::getStatus(){
    return status;
};

std::chrono::steady_clock::time_point Box::getActiveStartPoint(){
    return activeStartPoint;
};

void Box::addTicket(std::string newID, std::string newV, std::string newN, short s){
    //Ticket newT(newID, newV, newN, s);
    tickets.push_back(new Ticket(newID, newV, newN));

    //if box not already active, set active to true
    if(active == false){
        setActive(true);
    }
};

//takes string 'id' as a parameter
//if the id parameter matches the id of any ticket
//the first ticket instance that matched is removed
//if no match is found, nothing happens
void Box::removeTicket(std::string id){ //UNUSED: ticket object changed to tickets, method no longer usable
    for(unsigned short i = 0; i < tickets.size(); i++){
        if(id.compare(tickets[i]->getID()) == 0){
            tickets.erase(tickets.begin()+i);

            //if no ticket left, set active to false,
            //else, if removal index = 0, set active to true (resets timer)
            if(tickets.empty()){
                setActive(false);
            }else
            if(i == 0){
                setActive(true);
            }
            break;
        }
    }
};

void Box::removeTicketByIndex(unsigned short s){
    if(s >= 0 && s < tickets.size()){
        tickets.erase(tickets.begin()+s);

        //if no ticket left, set active to false,
        //else, if removal index = 0, set active to true (resets timer)
        if(tickets.empty()){
            setActive(false);
        }else
        if(s == 0){
            setActive(true);
        }
    }
}

std::string Box::getTimeSinceActiveAsString(std::chrono::steady_clock::time_point p){
    std::stringstream ss;

    //generate the lifespan duration of the bucket and generate the related data
    std::chrono::duration<int> lifespan = std::chrono::duration_cast<std::chrono::duration<int>>(p - activeStartPoint);
    int hours = lifespan.count() * std::chrono::hours::period::den / std::chrono::hours::period::num;
    int minutes = (lifespan.count() * std::chrono::minutes::period::den / std::chrono::minutes::period::num)%60;
    int seconds = lifespan.count() % 60;

    //depending on length of lifespan, add the duration data to the string stream
    if(hours > 0){
        ss << hours << "h ";
    }
    ss << minutes << "m " << seconds << "s";

    //return the final string
    return ss.str();
};
