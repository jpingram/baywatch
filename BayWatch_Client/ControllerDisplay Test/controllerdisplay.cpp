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

ControllerDisplay::ControllerDisplay(Controller& c):boxTextObjects(), boxRectObjects(),
        queueTextObjects(), queueRectObjects(), currentTimePoint(clock::now()){
    //SET UP FONTS
    freeroadR.loadFromMemory(freeroadRegular, freeroadRegular_length);
    freeroadB.loadFromMemory(freeroadBold, freeroadBold_length);

    createBoxObjects();
    createQueueObjects();

    update(Controller& c);
};

void ControllerDisplay::update(Controller& c){
    //update time objects
    currentTimePoint = clock::now();

    //update display objects
    updateBoxObjects(Controller& c);
    updateQueueObjects(Controller& c);
};

void ControllerDisplay::setRenderTarget(sf::RenderTarget &target){
    try{
        boxTextObjects.setTarget(target);
    }catch(const tgui::Exception &e){
        std::cerr << "TGUI Exception in Controller::setRenderTarget(): " << e.what() << std::endl;
    }
};

tgui::Gui* ControllerDisplay::getBoxTextObjects(){
    return &boxTextObjects;
};

std::vector<sf::RectangleShape> ControllerDisplay::getBoxRectObjects(){
    return boxRectObjects;
};

std::vector<sf::Text> ControllerDisplay::getQueueTextObjects(){
    return queueTextObjects;
};

std::vector<sf::RectangleShape> ControllerDisplay::getQueueRectObjects(){
    return queueRectObjects;
};

void ControllerDisplay::createBoxObjects(Controller& c){
    for(unsigned short i = 0; i < c.getBoxList()->size(); i++){
        Box test = c.getBoxList()->at(i);

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
            label->setSize(test.getBoundary()->getW(), TEXT_LINE_SPACING_21);
            label->getRenderer()->setTextColor(tgui::Color(sf::Color::White));
            ss.str(std::string());
            ss << "idLabel" << i;
            boxTextObjects.add(label, ss.str());

            //VEHICLE INFO
            label = tgui::Label::create();
            label->setText("Bay:");
            label->setTextSize(21);
            label->setPosition(test.getBoundary()->getX(), test.getBoundary()->getY() + 3*TEXT_LINE_SPACING_21);
            label->setSize(test.getBoundary()->getW(), 2*TEXT_LINE_SPACING_21);
            label->getRenderer()->setTextColor(tgui::Color(sf::Color::White));
            ss.str(std::string());
            ss << "vLabel" << i;
            boxTextObjects.add(label, ss.str());

            //NOTES INFO
            label = tgui::Label::create();
            label->setText("Bay:");
            label->setTextSize(21);
            label->setPosition(test.getBoundary()->getX(), test.getBoundary()->getY() + 5*TEXT_LINE_SPACING_21);
            label->setSize(test.getBoundary()->getW(), 6*TEXT_LINE_SPACING_21);
            label->getRenderer()->setTextColor(tgui::Color(sf::Color::White));
            ss.str(std::string());
            ss << "nLabel" << i;
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

            //BOX TIMER INFO
            label = tgui::Label::create();
            label->setText("");
            label->setTextSize(21);
            label->setPosition(test.getBoundary()->getX(), test.getBoundary()->getY() + BOX_HEIGHT - 2*TEXT_LINE_SPACING_21);
            label->setSize(tgui::Layout2d(tgui::Vector2f(BOX_WIDTH, TEXT_LINE_SPACING_21)));
            label->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Center);
            label->getRenderer()->setTextColor(tgui::Color(sf::Color::White));
            ss.str(std::string());
            ss << "timeLabel" << i;
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

void ControllerDisplay::updateBoxObjects(Controller& c){
    std::stringstream ss; //exists to help with print-formatting

    for(unsigned short i = 0; i < c.getBoxList()->size(); i++){
        //box boundary rectangle shape object
        //SET OUTLINE COLOR
        if(c.getBoxList()->at(i).isSelected()){
        boxRectObjects[i].setOutlineColor(sf::Color::White);
        }else
        if(c.getBoxList()->at(i).isActive()){
            if(c.getBoxList()->at(i).getTimeSinceActiveAsDouble(currentTimePoint) > BOX_URGENT_TIME){
                boxRectObjects[i].setOutlineColor(sf::Color::Red);
            }else
            if(c.getBoxList()->at(i).getTimeSinceActiveAsDouble(currentTimePoint) > BOX_WARNING_TIME){
                boxRectObjects[i].setOutlineColor(sf::Color::Yellow);
            }else{
                boxRectObjects[i].setOutlineColor(sf::Color::Green);
            }
        }else{
            boxRectObjects[i].setOutlineColor(sf::Color::Black);
        }
        //SET BACKGROUND COLOR
        if(c.getBoxList()->at(i).onFlicker(currentTimePoint)){
            boxRectObjects[i].setFillColor(sf::Color::Yellow);
        }else{
            boxRectObjects[i].setFillColor(sf::Color::Black);
        }

        //box label text
        ss.str(std::string());
        ss << "bayLabel" << i;
        //SET COLOR
        if(c.getBoxList()->at(i).onFlicker(currentTimePoint)){
            std::dynamic_pointer_cast<tgui::Label>(boxTextObjects.get(ss.str()))
                ->getRenderer()->setTextColor(tgui::Color(sf::Color::Black));
        }else{
            std::dynamic_pointer_cast<tgui::Label>(boxTextObjects.get(ss.str()))
                ->getRenderer()->setTextColor(tgui::Color(sf::Color::White));
        }
        //SET TEXT
        std::dynamic_pointer_cast<tgui::Label>(boxTextObjects.get(ss.str()))->setText(boxList[i].getLabel());


        //ticket ID text
        ss.str(std::string());
        ss << "idLabel" << i;
        //SET COLOR
        if(c.getBoxList()->at(i).onFlicker(currentTimePoint)){
            std::dynamic_pointer_cast<tgui::Label>(boxTextObjects.get(ss.str()))
                ->getRenderer()->setTextColor(tgui::Color(sf::Color::Black));
        }else{
            std::dynamic_pointer_cast<tgui::Label>(boxTextObjects.get(ss.str()))
                ->getRenderer()->setTextColor(tgui::Color(sf::Color::White));
        }
        //SET TEXT
        if(c.getBoxList()->at(i).getTickets().empty()){
            std::dynamic_pointer_cast<tgui::Label>(boxTextObjects.get(ss.str()))->setText("");
        }else{
            std::dynamic_pointer_cast<tgui::Label>(boxTextObjects.get(ss.str()))->setText(boxList[i].getTickets()[0]->getID());
        }

        //vehicle text
        ss.str(std::string());
        ss << "vLabel" << i;
        //SET COLOR
        if(c.getBoxList()->at(i).onFlicker(currentTimePoint)){
            std::dynamic_pointer_cast<tgui::Label>(boxTextObjects.get(ss.str()))
                ->getRenderer()->setTextColor(tgui::Color(sf::Color::Black));
        }else{
            std::dynamic_pointer_cast<tgui::Label>(boxTextObjects.get(ss.str()))
                ->getRenderer()->setTextColor(tgui::Color(sf::Color::White));
        }
        //SET TEXT
        if(c.getBoxList()->at(i).getTickets().empty()){
            std::dynamic_pointer_cast<tgui::Label>(boxTextObjects.get(ss.str()))->setText("");
        }else{
            std::dynamic_pointer_cast<tgui::Label>(boxTextObjects.get(ss.str()))->setText(boxList[i].getTickets()[0]->getVehicle());
        }

        ss.str(std::string());
        ss << "nLabel" << i;
        //SET COLOR
        if(c.getBoxList()->at(i).onFlicker(currentTimePoint)){
            std::dynamic_pointer_cast<tgui::Label>(boxTextObjects.get(ss.str()))
                ->getRenderer()->setTextColor(tgui::Color(sf::Color::Black));
        }else{
            std::dynamic_pointer_cast<tgui::Label>(boxTextObjects.get(ss.str()))
                ->getRenderer()->setTextColor(tgui::Color(sf::Color::White));
        }
        //SET TEXT
        if(c.getBoxList()->at(i).getTickets().empty()){
            std::dynamic_pointer_cast<tgui::Label>(boxTextObjects.get(ss.str()))->setText("");
        }else{
            std::dynamic_pointer_cast<tgui::Label>(boxTextObjects.get(ss.str()))->setText(boxList[i].getTickets()[0]->getNotes());
        }

        //vehicle waiting label
        ss.str(std::string());
        ss << "vwLabel" << i;
        //SET COLOR
        if(c.getBoxList()->at(i).onFlicker(currentTimePoint)){
            std::dynamic_pointer_cast<tgui::Label>(boxTextObjects.get(ss.str()))
                ->getRenderer()->setTextColor(tgui::Color(sf::Color::Black));
        }else{
            std::dynamic_pointer_cast<tgui::Label>(boxTextObjects.get(ss.str()))
                ->getRenderer()->setTextColor(tgui::Color(sf::Color::White));
        }
        //SET TEXT (changes contents of 'ss')
        if(c.getBoxList()->at(i).getTickets().size() > 1){
            std::string temp = ss.str(); //holds the name of the vwLabel for later use
            ss.str(std::string());
            ss << "Vehicles Waiting: " << boxList[i].getTickets().size()-1;
            std::dynamic_pointer_cast<tgui::Label>(boxTextObjects.get(temp))
                ->setText(ss.str());
        }else{
            std::dynamic_pointer_cast<tgui::Label>(boxTextObjects.get(ss.str()))
                ->setText("");
        }

        //bay active time label
        ss.str(std::string());
        ss << "timeLabel" << i;
        //SET COLOR
        if(c.getBoxList()->at(i).onFlicker(currentTimePoint)){
            std::dynamic_pointer_cast<tgui::Label>(boxTextObjects.get(ss.str()))
                ->getRenderer()->setTextColor(tgui::Color(sf::Color::Black));
        }else{
            std::dynamic_pointer_cast<tgui::Label>(boxTextObjects.get(ss.str()))
                ->getRenderer()->setTextColor(tgui::Color(sf::Color::White));
        }
        //SET TEXT (changes contents of 'ss')
        if(c.getBoxList()->at(i).isActive()){
            std::string temp = ss.str(); //holds the name of the timeLabel for later use
            ss.str(std::string());
            ss << "Bay Active: " << boxList[i].getTimeSinceActiveAsString(currentTimePoint);
            std::dynamic_pointer_cast<tgui::Label>(boxTextObjects.get(temp))
                ->setText(ss.str());
        }else{
            std::dynamic_pointer_cast<tgui::Label>(boxTextObjects.get(ss.str()))
                ->setText("");
        }
    }
};

void ControllerDisplay::createQueueObjects(Controller& c){
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

void ControllerDisplay::updateQueueObjects(Controller& c){
    std::stringstream ss; //exists to help with print-formatting
    unsigned short i = 0;
    while(i < ticketQueue.size()){
        if(ticketQueue[i].onFlicker(currentTimePoint)){
            queueTextObjects[i].setFillColor(sf::Color::Black);
            queueRectObjects[i].setFillColor(sf::Color::Yellow);
        }else
        if(i == selectedBucket){
            queueTextObjects[i].setFillColor(sf::Color::Black);
            queueRectObjects[i].setFillColor(sf::Color::White);
        }else{
            queueTextObjects[i].setFillColor(sf::Color::White);
            queueRectObjects[i].setFillColor(sf::Color::Black);
        }

        ss.str(std::string());
        ss << (i+1);
        if(ticketQueue[i].getBoxNum() >= 0){
            ss << " [" << boxList[ticketQueue[i].getBoxNum()].getLabel() << "]";
        }
        ss << ": " << ticketQueue[i].getTicket()->getID() << " - " << ticketQueue[i].getTicket()->getVehicle() << " - ";
        if(ticketQueue[i].getTicket()->getNotes().size() > QUEUE_NOTES_LIMIT){
            ss << ticketQueue[i].getTicket()->getNotes().substr(0, QUEUE_NOTES_LIMIT) << "...";
        }else{
            ss << ticketQueue[i].getTicket()->getNotes();
        }
        ss << " - " << ticketQueue[i].getTimeSinceBirthAsString(currentTimePoint);
        queueTextObjects[i].setString(ss.str());
        i++;
    }
    while(i < QUEUE_TICKET_LIMIT){
        queueRectObjects[i].setFillColor(sf::Color::Black);
        queueTextObjects[i].setString("");
        i++;
    }
};
