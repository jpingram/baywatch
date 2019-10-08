#include <chrono>
#include <string>
#include <sstream>
#include <cmath>
#include "ticket.h"
#include "ticketbucket.h"

TicketBucket::TicketBucket():ticket(), boxNum(), selected(false), active(false),
        birthPoint(std::chrono::steady_clock::now()), mostRecentUpdatePoint(std::chrono::steady_clock::now()){};

TicketBucket::TicketBucket(std::string newID, std::string newV, std::string newN):
        ticket(newID, newV, newN), boxNum(), selected(false), active(false),
        birthPoint(std::chrono::steady_clock::now()), mostRecentUpdatePoint(std::chrono::steady_clock::now()){};

TicketBucket::TicketBucket(std::string newID, std::string newV, std::string newN, short s):
        ticket(newID, newV, newN), boxNum(s), selected(false), active(false),
        birthPoint(std::chrono::steady_clock::now()), mostRecentUpdatePoint(std::chrono::steady_clock::now()){};

void TicketBucket::updateTicket(std::string newID, std::string newV, std::string newN){
    if(ticket.getID().compare(newID) != 0 || ticket.getVehicle().compare(newV) != 0 || ticket.getNotes().compare(newN) != 0){
        setMostRecentUpdatePoint(std::chrono::steady_clock::now());
    }

    ticket.setID(newID);
    ticket.setVehicle(newV);
    ticket.setNotes(newN);
};

void TicketBucket::setBoxNum(short s){
    if(boxNum != s){
        setMostRecentUpdatePoint(std::chrono::steady_clock::now());
    }

    boxNum = s;
};

void TicketBucket::setSelected(bool b){
    selected = b;
};

void TicketBucket::setActive(bool b){
    active = b;
};

void TicketBucket::setMostRecentUpdatePoint(std::chrono::steady_clock::time_point p){
    mostRecentUpdatePoint = p;
};

void TicketBucket::setBirthPoint(std::chrono::steady_clock::time_point p){
    birthPoint = p;
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

std::chrono::duration<long> TicketBucket::getTimeSinceBirth(std::chrono::steady_clock::time_point p){
    return std::chrono::duration_cast<std::chrono::duration<long>>(p - birthPoint);
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

bool TicketBucket::onFlicker(std::chrono::steady_clock::time_point p){
    std::chrono::duration<double> timeSinceUpdate
        = std::chrono::duration_cast<std::chrono::duration<double>>(p - mostRecentUpdatePoint);

    if(timeSinceUpdate.count() < fullUpdateFlickeringDuration.count()){
        if(fmod(timeSinceUpdate.count(), (2*singleFlickerDuration.count())) < singleFlickerDuration.count()){
            return true;
        }else{
            return false;
        }
    }

    return false;
};
