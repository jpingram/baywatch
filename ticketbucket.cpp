#include "ticket.h"
#include "ticketbucket.h"

TicketBucket::TicketBucket():ticket(), boxNum(), selected(false){};

TicketBucket::TicketBucket(std::string newID, std::string newV, std::string newN):
        ticket(newID, newV, newN), boxNum(), selected(false){};

TicketBucket::TicketBucket(std::string newID, std::string newV, std::string newN, short s):
        ticket(newID, newV, newN), boxNum(s), selected(false){};

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

Ticket* TicketBucket::getTicket(){
    return &ticket;
};

short TicketBucket::getBoxNum(){
    return boxNum;
};

bool TicketBucket::isSelected(){
    return selected;
};
