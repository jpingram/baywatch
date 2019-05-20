#include <chrono>
#include <string>
#include <sstream>
#include "ticket.h"
#include "ticketbucket.h"

TicketBucket::TicketBucket():ticket(), boxNum(), selected(false), active(false),
        birthPoint(std::chrono::steady_clock::now()){};

TicketBucket::TicketBucket(std::string newID, std::string newV, std::string newN):
        ticket(newID, newV, newN), boxNum(), selected(false), active(false), birthPoint(std::chrono::steady_clock::now()){};

TicketBucket::TicketBucket(std::string newID, std::string newV, std::string newN, short s):
        ticket(newID, newV, newN), boxNum(s), selected(false), active(false), birthPoint(std::chrono::steady_clock::now()){};

void TicketBucket::updateTicket(std::string newID, std::string newV, std::string newN){
    ticket.setID(newID);
    ticket.setVehicle(newV);
    ticket.setNotes(newN);
};

void TicketBucket::setBoxNum(short s){
    boxNum = s;
};

void TicketBucket::setSelected(bool b){
    selected = b;
};

void TicketBucket::setActive(bool b){
    active = b;
};

Ticket* TicketBucket::getTicket(){
    return &ticket;
};

short TicketBucket::getBoxNum(){
    return boxNum;
};

bool TicketBucket::isSelected(){
    return selected;
};

bool TicketBucket::isActive(){
    return active;
};

void TicketBucket::setBirthPoint(std::chrono::steady_clock::time_point p){
    birthPoint = p;
};

std::string TicketBucket::getTimeSinceBirthAsString(std::chrono::steady_clock::time_point p){
    std::stringstream ss;

    //generate the lifespan duration of the bucket and generate the related data
    std::chrono::duration<int> lifespan = std::chrono::duration_cast<std::chrono::duration<int>>(p - birthPoint);
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
