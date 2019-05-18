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
#include "controller.h"

Controller::Controller():boxList(), ticketQueue(), boxTextObjects(), boxRectObjects(),
        queueTextObjects(), queueRectObjects(), status(0), selectedBox(-1), selectedBucket(-1),
        currentTimePoint(clock::now()){
    //SET UP FONTS
    freeroadR.loadFromMemory(freeroadRegular, freeroadRegular_length);
    freeroadB.loadFromMemory(freeroadBold, freeroadBold_length);

    createBoxObjects();
    createQueueObjects();

    update();
};

void Controller::setRenderTarget(sf::RenderTarget &target){
    try{
        boxTextObjects.setTarget(target);
    }catch(const tgui::Exception &e){
        std::cerr << "TGUI Exception in Controller::setRenderTarget(): " << e.what() << std::endl;
    }
};

std::vector<Box>* Controller::getBoxList(){
    return &boxList;
};

std::vector<TicketBucket>* Controller::getTicketQueue(){
    return &ticketQueue;
};

tgui::Gui* Controller::getBoxTextObjects(){
    return &boxTextObjects;
};

std::vector<sf::RectangleShape> Controller::getBoxRectObjects(){
    return boxRectObjects;
};

std::vector<sf::Text> Controller::getQueueTextObjects(){
    return queueTextObjects;
};

std::vector<sf::RectangleShape> Controller::getQueueRectObjects(){
    return queueRectObjects;
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
        if(ticketQueue[index].getBoxNum() != -1){
            //if the bucket is already assigned to the target box, abort
            if(ticketQueue[index].getBoxNum() == newBox){
                return;
            }

            //else, remove target ticket from it's current box
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
                    boxList[boxIndex].getTickets()[i]->setID(newID);
                    boxList[boxIndex].getTickets()[i]->setVehicle(newV);
                    boxList[boxIndex].getTickets()[i]->setNotes(newN);
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

void Controller::update(){
    //update time objects
    currentTimePoint = clock::now();

    //update display objects
    updateBoxObjects();
    updateQueueObjects();
};

void Controller::createBoxObjects(){
    for(unsigned short i = 0; i < 5; i++){
        std::stringstream ss;
        ss << "Bay" << (i+1);
        Box test(ss.str(), BOX_X_ZERO_POINT+(i*BOX_WIDTH)+(i*10), BOX_Y_ZERO_POINT, BOX_WIDTH, BOX_HEIGHT);
        boxList.push_back(test);

        /*sf::Text indexText;
        indexText.setFont(freeroadB);
        indexText.setCharacterSize(15);
        indexText.setPosition(test.getBoundary()->getX(), test.getBoundary()->getY());
        indexText.setFillColor(sf::Color::White);
        boxTextObjects.push_back(indexText);

        sf::Text idText;
        idText.setFont(freeroadR);
        idText.setCharacterSize(15);
        idText.setPosition(test.getBoundary()->getX(), test.getBoundary()->getY() + TEXT_LINE_SPACING_15);
        idText.setFillColor(sf::Color::White);
        boxTextObjects.push_back(idText);

        sf::Text vText;
        vText.setFont(freeroadR);
        vText.setCharacterSize(15);
        vText.setPosition(test.getBoundary()->getX(), test.getBoundary()->getY() + 2*TEXT_LINE_SPACING_15);
        vText.setFillColor(sf::Color::White);
        boxTextObjects.push_back(vText);*/

        try{
            boxTextObjects.setFont(freeroadB);

            //BAY LABEL
            auto label = tgui::Label::create();
            label->setText("Bay:");
            label->setTextSize(27);
            label->setPosition(test.getBoundary()->getX(), test.getBoundary()->getY());
            label->setSize(tgui::Layout2d(tgui::Vector2f(BOX_WIDTH, TEXT_LINE_SPACING_27)));
            label->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Center);
            label->getRenderer()->setTextColor(tgui::Color(sf::Color::White));
            ss.str(std::string());
            ss << "bayLabel" << i;
            boxTextObjects.add(label, ss.str());

            //TICKET ID
            label = tgui::Label::create();
            label->setText("Bay:");
            label->setTextSize(21);
            label->setPosition(test.getBoundary()->getX(), test.getBoundary()->getY() + 2*TEXT_LINE_SPACING_21);
            label->getRenderer()->setTextColor(tgui::Color(sf::Color::White));
            ss.str(std::string());
            ss << "idLabel" << i;
            boxTextObjects.add(label, ss.str());

            //VEHICLE INFO
            label = tgui::Label::create();
            label->setText("Bay:");
            label->setTextSize(21);
            label->setPosition(test.getBoundary()->getX(), test.getBoundary()->getY() + 3*TEXT_LINE_SPACING_21);
            label->getRenderer()->setTextColor(tgui::Color(sf::Color::White));
            ss.str(std::string());
            ss << "vLabel" << i;
            boxTextObjects.add(label, ss.str());

            //VEHICLE WAITING INFO
            label = tgui::Label::create();
            label->setText("");
            label->setTextSize(21);
            label->setPosition(test.getBoundary()->getX(), test.getBoundary()->getY() + BOX_HEIGHT - TEXT_LINE_SPACING_21);
            label->setSize(tgui::Layout2d(tgui::Vector2f(BOX_WIDTH, TEXT_LINE_SPACING_21)));
            label->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Center);
            label->getRenderer()->setTextColor(tgui::Color(sf::Color::White));
            ss.str(std::string());
            ss << "vwLabel" << i;
            boxTextObjects.add(label, ss.str());
        }catch(const tgui::Exception &e){
            std::cerr << "TGUI Exception in Controller::createBoxObjects(): " << e.what() << std::endl;
        }

        sf::RectangleShape boxBoundary;
        boxBoundary.setSize(sf::Vector2f(test.getBoundary()->getW(), test.getBoundary()->getH()));
        boxBoundary.setPosition(test.getBoundary()->getX(), test.getBoundary()->getY());
        boxBoundary.setFillColor(sf::Color::Black);
        boxBoundary.setOutlineColor(sf::Color::White);
        boxBoundary.setOutlineThickness(5.0f);
        boxRectObjects.push_back(boxBoundary);
    }
};

void Controller::updateBoxObjects(){
    std::stringstream ss; //exists to help with print-formatting

    for(unsigned short i = 0; i < boxList.size(); i++){
        //box boundary rectangle shape object
        if(boxList[i].isSelected()){
        boxRectObjects[i].setOutlineColor(sf::Color::Red);
        }else{
            boxRectObjects[i].setOutlineColor(sf::Color::White);
        }

        //box label text
        //SET FONT
        /*if(i == selectedBox && boxList[selectedBox].getStatus() != selectStatuses::stage1){
            boxTextObjects[i*3].setFont(freeroadR);
        }else{
            boxTextObjects[i*3].setFont(freeroadB);
        }
        //SET TEXT
        boxTextObjects[i*3].setString(boxList[i].getLabel());*/
        ss.str(std::string());
        ss << "bayLabel" << i;
        std::dynamic_pointer_cast<tgui::Label>(boxTextObjects.get(ss.str()))->setText(boxList[i].getLabel());

        //ticket ID text
        //SET FONT
        /*if(i == selectedBox && boxList[selectedBox].getStatus() == selectStatuses::stage2){
            boxTextObjects[i*3+1].setFont(freeroadB);
        }else{
            boxTextObjects[i*3+1].setFont(freeroadR);
        }
        //SET TEXT
        if(boxList[i].getTickets().empty()){
            boxTextObjects[i*3+1].setString("...");
        }else{
            boxTextObjects[i*3+1].setString(boxList[i].getTickets()[0]->getID());
        }*/
        ss.str(std::string());
        ss << "idLabel" << i;
        if(boxList[i].getTickets().empty()){
            std::dynamic_pointer_cast<tgui::Label>(boxTextObjects.get(ss.str()))->setText("...");
        }else{
            std::dynamic_pointer_cast<tgui::Label>(boxTextObjects.get(ss.str()))->setText(boxList[i].getTickets()[0]->getID());
        }

        //vehicle text
        //SET FONT
        /*if(i == selectedBox && boxList[selectedBox].getStatus() == selectStatuses::stage3){
            boxTextObjects[i*3+2].setFont(freeroadB);
        }else{
            boxTextObjects[i*3+2].setFont(freeroadR);
        }
        //SET TEXT
        if(boxList[i].getTickets().empty()){
            boxTextObjects[i*3+2].setString("VACANT");
        }else{
            boxTextObjects[i*3+2].setString(boxList[i].getTickets()[0]->getVehicle());
        }*/
        ss.str(std::string());
        ss << "vLabel" << i;
        if(boxList[i].getTickets().empty()){
            std::dynamic_pointer_cast<tgui::Label>(boxTextObjects.get(ss.str()))->setText("...");
        }else{
            std::dynamic_pointer_cast<tgui::Label>(boxTextObjects.get(ss.str()))->setText(boxList[i].getTickets()[0]->getVehicle());
        }

        ss.str(std::string());
        ss << "vwLabel" << i;
        if(boxList[i].getTickets().size() > 1){
            std::string temp = ss.str(); //holds the name of the vwLabel for later use
            ss.str(std::string());
            ss << "Vehicles Waiting: " << boxList[i].getTickets().size()-1;
            std::dynamic_pointer_cast<tgui::Label>(boxTextObjects.get(temp))
                ->setText(ss.str());
        }else{
            std::dynamic_pointer_cast<tgui::Label>(boxTextObjects.get(ss.str()))
                ->setText("");
        }
    }
};

void Controller::createQueueObjects(){
    for(unsigned short i = 0; i < QUEUE_TICKET_LIMIT; i++){
        sf::Text tText;
        tText.setFont(freeroadR);
        tText.setCharacterSize(15);
        tText.setPosition(QUEUE_X_ZERO_POINT, QUEUE_Y_ZERO_POINT + i*TEXT_LINE_SPACING_15);
        tText.setFillColor(sf::Color::White);
        queueTextObjects.push_back(tText);

        sf::RectangleShape ticketBacking;
        ticketBacking.setSize(sf::Vector2f(QUEUE_WIDTH, TEXT_LINE_SPACING_15));
        ticketBacking.setPosition(QUEUE_X_ZERO_POINT, QUEUE_Y_ZERO_POINT + i*TEXT_LINE_SPACING_15);
        ticketBacking.setFillColor(sf::Color::Black);
        queueRectObjects.push_back(ticketBacking);
    }
};

void Controller::updateQueueObjects(){
    std::stringstream ss; //exists to help with print-formatting
    unsigned short i = 0;
    while(i < ticketQueue.size()){
        if(i == selectedBucket){
            queueTextObjects[i].setFillColor(sf::Color::Black);
            queueRectObjects[i].setFillColor(sf::Color::White);
        }else{
            queueTextObjects[i].setFillColor(sf::Color::White);
            queueRectObjects[i].setFillColor(sf::Color::Black);
        }

        ss.str(std::string());
        ss << (i+1) << ": " << ticketQueue[i].getTicket()->getID() << " - " <<ticketQueue[i].getTicket()->getVehicle() <<
            " - " << ticketQueue[i].getTicket()->getNotes() <<
            " - " << ticketQueue[i].getTimeSinceBirthAsString(currentTimePoint);
        queueTextObjects[i].setString(ss.str());
        i++;
    }
    while(i < QUEUE_TICKET_LIMIT){
        queueRectObjects[i].setFillColor(sf::Color::Black);
        queueTextObjects[i].setString("");
        i++;
    }
};
