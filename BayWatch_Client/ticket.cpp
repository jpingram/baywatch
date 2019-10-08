#include <string>
#include "ticket.h"

Ticket::Ticket():id("0"), vehicle("NEED INFO"), notes(""){};

Ticket::Ticket(std::string newID):id(newID), vehicle("NEED INFO"), notes(""){};

Ticket::Ticket(std::string newID, std::string newV):id(newID), vehicle(newV), notes(""){};

Ticket::Ticket(std::string newID, std::string newV, std::string newN):id(newID), vehicle(newV), notes(newN){};

void Ticket::setID(std::string newID){
    id = newID;
};

void Ticket::setVehicle(std::string newV){
    vehicle = newV;
    if(vehicle.size() > VEHICLE_CHAR_LIMIT){
        vehicle = vehicle.substr(0, VEHICLE_CHAR_LIMIT);
    }
};

void Ticket::setNotes(std::string newN){
    notes = newN;
    if(notes.size() > NOTES_CHAR_LIMIT){
        notes = notes.substr(0, NOTES_CHAR_LIMIT);
    }
};

std::string Ticket::getID(){
    return id;
};

std::string Ticket::getVehicle(){
    return vehicle;
};

std::string Ticket::getNotes(){
    return notes;
};
