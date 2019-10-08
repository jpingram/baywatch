#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <memory>
#include <chrono>
#include <TGUI/Gui.hpp>
#include <TGUI/Layout.hpp>
#include <TGUI/Vector2f.hpp>
#include <TGUI/Renderers/LabelRenderer.hpp>
#include <TGUI/Widgets/Label.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include "data.h"
#include "box.h"
#include "ticketbucket.h"

Controller::Controller():boxList(), ticketQueue(), boxTextObjects(), boxRectObjects(),
        queueTextObjects(), queueRectObjects(), status(0), selectedBox(-1), selectedBucket(-1),
        currentTimePoint(clock::now()){

    for(unsigned short i = 0; i < 5; i++){
        std::stringstream ss;
        ss << "Bay" << (i+1);
        Box test(ss.str(), BOX_X_ZERO_POINT+(i*BOX_WIDTH)+(i*10), BOX_Y_ZERO_POINT, BOX_WIDTH, BOX_HEIGHT);
        boxList.push_back(test);
    }
};

std::vector<Box>* Controller::getBoxList(){
    return &boxList;
};

std::vector<TicketBucket>* Controller::getTicketQueue(){
    return &ticketQueue;
};

bool Controller::getStatus(){
    return status;
};

void Controller::setStatus(unsigned short s){
    status = s;
};

short Controller::isInBox(short x, short y){
    for(unsigned short i = 0; i < boxList.size(); i++){
        if(boxList.at(i).getBoundary()->containsPoint(x, y)){
            return i;
        }
    }

    return -1;
};

void Controller::selectBox(short s){
    //if selectedBox does not equal current box-to-select
    //      clear initial box selection
    //      set new selected box
    if(selectedBox == s){
        boxList[s].setSelected(true);
    }else{
        deselectBox();

        //if s is a legal box index, select the box at that index; else, do nothing
        if(s >= 0 && s < (short)boxList.size()){
            boxList[s].setSelected(true);
            selectedBox = s;
            setStatus(conStatuses::boxSelected);
        }else{
            selectedBox = -1;
        }
    }
};

void Controller::deselectBox(){
    if(selectedBox != -1){
        boxList[selectedBox].setSelected(false);
        setStatus(conStatuses::noneSelected);
    }
    selectedBox = -1;
};

short Controller::getSelectedBox(){
    return selectedBox;
};

short Controller::isInBucket(short x, short y){
    if(ticketQueue.empty()){
        return -1;
    }

    Rect queueBoundary(QUEUE_X_ZERO_POINT, QUEUE_Y_ZERO_POINT, QUEUE_WIDTH, TEXT_LINE_SPACING_15*ticketQueue.size());
    if(queueBoundary.containsPoint(x, y)){
        return (y-QUEUE_Y_ZERO_POINT)/TEXT_LINE_SPACING_15;
    }

    return -1;
};

void Controller::selectBucket(short s){
    //if selectedBucket does not equal current box-to-select
    //      clear initial bucket selection
    //      set new selected bucket
    if(selectedBucket == s){
        ticketQueue[s].setSelected(true);
    }else{
        deselectBucket();

        //if s is a legal bucket index, select the bucket at that index; else, do nothing
        if(s >= 0 && s < (short)ticketQueue.size()){
            ticketQueue[s].setSelected(true);
            selectedBucket = s;
            setStatus(conStatuses::bucketSelected);
        }else{
            selectedBucket = -1;
        }
    }
};

void Controller::deselectBucket(){
    if(selectedBucket != -1){
        ticketQueue[selectedBucket].setSelected(false);
        setStatus(conStatuses::noneSelected);
    }
    selectedBucket = -1;
};

short Controller::getSelectedBucket(){
    return selectedBucket;
};

//returns index of bucket in ticketQueue or -1 if no bucket with matching id is found
short Controller::getBucketIndexById(std::string id){
    for(unsigned short i = 0; i < ticketQueue.size(); i++){
        if(id.compare(ticketQueue.at(i).getTicket()->getID()) == 0){
            return i;
        }
    }

    return -1;
};

//TICKET RULES
//  -all ticket IDs must be unique for ticket to be added
//  -also boxNum values must be between [-1, boxList.size())
//functions addTicket returns 0 if add was successful and -1 if not
short Controller::addTicket(std::string newID, std::string newV, std::string newN, short s){
    if(isIdUnique(newID)){
        //clean boxNum
        short newS = s;
        if(newS < -1 || newS >= (short)boxList.size()){
            newS = -1;
        }

        //create ticket bucket
        TicketBucket tick(newID, newV, newN, newS);
        ticketQueue.push_back(tick);

        //if ticket in bucket has a valid box number, add reference to ticket in box
        if(newS != -1){
            boxList[newS].addTicket(newID, newV, newN, newS);
        }
    }else{
        return -1;
    }

    return 0;
};

//checks the ticket queue and compares parameter 'id' to
//the IDs of all current tickets in the queue
//returns true if 'id' is unique, false if not
bool Controller::isIdUnique(std::string id){
    for(unsigned short i = 0; i < ticketQueue.size(); i++){
        if(id.compare(ticketQueue[i].getTicket()->getID()) == 0){
            return false;
        }
    }
    return true;
};

void Controller::setBoxNum(unsigned short index, short newBox){
    //if index is a legal and active index,
    //  if ticket bucket at index has ticket already in a box,
    //      remove ticket from box
    //  change boxNum value in bucket
    //  add ticket to box at boxList[newBox]
    if(index >= -1 && index < (short)ticketQueue.size()){
        //std::chrono::steady_clock::time_point tp(currentTimePoint); //used to store old box timer info if applicable

        if(ticketQueue[index].getBoxNum() != -1){
            //if the bucket is already assigned to the target box, abort
            if(ticketQueue[index].getBoxNum() == newBox){
                return;
            }

            //else, remove target ticket from it's current box
            //tp = boxList[ticketQueue[index].getBoxNum()].getActiveStartPoint();
            boxList[ticketQueue[index].getBoxNum()].removeTicket(ticketQueue[index].getTicket()->getID());
        }

        ticketQueue[index].setBoxNum(newBox);

        //if ticket in bucket has a valid box number, add reference to ticket in box
        if(newBox != -1){
            Ticket* t = ticketQueue[index].getTicket();
            boxList[newBox].addTicket(t->getID(), t->getVehicle(), t->getNotes(), newBox);
        }
    }

};

void Controller::updateTicket(short index, std::string newID, std::string newV, std::string newN){
    if(index >= -1 && index < (short)ticketQueue.size()){
        short boxIndex = ticketQueue[index].getBoxNum();
        if(boxIndex != -1){
            //find the ticket and then update ticket info
            for(unsigned short i = 0; i < boxList[boxIndex].getTickets().size(); i++){
                //if ID of ticket in bucket matches id of ticket in box, update info
                if(ticketQueue[index].getTicket()->getID().compare(boxList[boxIndex].getTickets()[i]->getID()) == 0){
                    boxList[boxIndex].updateTicketAtIndex(i, newID, newV, newN);
                    break;
                }
            }
        }

        ticketQueue[index].updateTicket(newID, newV, newN);
    }
};

void Controller::removeTicket(unsigned short s){
    //if s is a legal and active ticket bucket index number,
    //  deselect ticketBucket, if applicable
    //  if the ticket in the bucket is referenced by a box
    //      remove that reference from that box
    //  remove the ticket bucket from queue
    if(s < ticketQueue.size()){
        deselectBucket();
        if(ticketQueue[s].getBoxNum() != -1){
            boxList[ticketQueue[s].getBoxNum()].removeTicket(ticketQueue[s].getTicket()->getID());
        }
        ticketQueue.erase(ticketQueue.begin()+s);
    }
};

void Controller::removeActiveTicketFromBox(unsigned short s){
    if(s >= 0 && s < boxList.size()){
        if(!boxList[s].getTickets().empty()){
            //assign bucket of ticket in question to box -1 (none)
            //this also removes ticket object from box (see setBoxNum())
            short bucketIndex = getBucketIndexById(boxList[s].getTickets().at(0)->getID());
            if(bucketIndex >= 0){ //check to make sure bucket was actually found
                setBoxNum(bucketIndex, -1);
            }
        }
    }
};

void Controller::removeAllTicketsFromBox(unsigned short s){
    if(s >= 0 && s < boxList.size()){
        while(!boxList[s].getTickets().empty()){
            //assign bucket of ticket in question to box -1 (none)
            //this also removes ticket object from box (see setBoxNum())
            short bucketIndex = getBucketIndexById(boxList[s].getTickets().at(0)->getID());
            if(bucketIndex >= 0){ //check to make sure bucket was actually found
                setBoxNum(bucketIndex, -1);
            }
        }
    }
};
