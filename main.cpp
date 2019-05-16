#include <TGUI/TGUI.hpp>
#include <SFML/Graphics.hpp>
#include <string>
#include <sstream>
#include <iostream>
#include <memory>
#include "data.h"
#include "controller.h"

#define MAIN_WINDOW_WIDTH 1200
#define MAIN_WINDOW_HEIGHT 600

#define vwindow std::dynamic_pointer_cast<tgui::ChildWindow>(gui.get("child"))
#define menubar std::dynamic_pointer_cast<tgui::MenuBar>(gui.get("menu"))
#define selectedBox c.getBoxList().at(c.getSelectedBox())
#define selectedBucket c.getTicketQueue().at(c.getSelectedBucket())

enum windowStatuses{base, add, added, edit, edited, cancelled};
static unsigned short status = windowStatuses::base;
enum windowType{a, e};

void newVehicleWindow(tgui::Gui& gui, Controller& c, unsigned short t){
    try{
        //CHILDWINDOW
        auto child = tgui::ChildWindow::create();
        //child->setRenderer(theme.getRenderer("ChildWindow"));
        child->setSize(500, 300);
        child->setPosition(200, 200);
        if(t == windowType::a){
            child->setTitle("New Ticket Info");
        }else
        if(t == windowType::e){
            child->setTitle("Edit Ticket Info");
        }
        gui.add(child, "child");

        //LABEL
        auto label = tgui::Label::create();
        //label->setRenderer(theme.getRenderer("Label"));
        label->setText("Ticket ID:");
        label->setPosition(10, 30);
        label->setTextSize(15);
        child->add(label, "idLabel");

        //EDITBOX
        auto editBox = tgui::EditBox::create();
        //editBox->setRenderer(theme.getRenderer("EditBox"));
        editBox->setSize(365, 25);
        editBox->setTextSize(18);
        editBox->setPosition(125, 30);
        if(t == windowType::e){
            if(c.getSelectedBucket() >= 0){
                editBox->setDefaultText(selectedBucket.getTicket()->getID());
            }else{
                editBox->setDefaultText("ERROR: invalid selection");
            }
        }else{
            editBox->setDefaultText("");
        }
        child->add(editBox, "idInput");

        //LABEL
        label = tgui::Label::create();
        //label->setRenderer(theme.getRenderer("Label"));
        label->setText("Vehicle Info:");
        label->setPosition(10, 60);
        label->setTextSize(15);
        child->add(label, "vehicleLabel");

        //EDITBOX
        editBox = tgui::EditBox::create();
        //editBox->setRenderer(theme.getRenderer("EditBox"));
        editBox->setSize(365, 25);
        editBox->setTextSize(18);
        editBox->setPosition(125, 60);
        if(t == windowType::e){
            if(c.getSelectedBucket() >= 0){
                editBox->setDefaultText(selectedBucket.getTicket()->getVehicle());
            }else{
                editBox->setDefaultText("ERROR: invalid selection");
            }
        }else{
            editBox->setDefaultText("");
        }
        child->add(editBox, "vehicleInput");

        //LABEL
        label = tgui::Label::create();
        //label->setRenderer(theme.getRenderer("Label"));
        label->setText("Notes:");
        label->setPosition(10, 90);
        label->setTextSize(15);
        child->add(label, "notesLabel");

        //EDITBOX
        editBox = tgui::EditBox::create();
        //editBox->setRenderer(theme.getRenderer("EditBox"));
        editBox->setSize(365, 25);
        editBox->setTextSize(18);
        editBox->setPosition(125, 90);
        if(t == windowType::e){
            if(c.getSelectedBucket() >= 0){
                editBox->setDefaultText(selectedBucket.getTicket()->getNotes());
            }else{
                editBox->setDefaultText("ERROR: invalid selection");
            }
        }else{
            editBox->setDefaultText("");
        }
        child->add(editBox, "notesInput");

        //LABEL
        label = tgui::Label::create();
        //label->setRenderer(theme.getRenderer("Label"));
        label->setText("Bay:");
        label->setPosition(10, 120);
        label->setTextSize(15);
        child->add(label, "bayLabel");

        //combobox for now
        auto comboBox = tgui::ComboBox::create();
        //comboBox->setRenderer(theme.getRenderer("ComboBox"));
        comboBox->setSize(120, 30);
        comboBox->setPosition(50, 120);
        comboBox->addItem("[none]");
        comboBox->addItem("Bay 1");
        comboBox->addItem("Bay 2");
        comboBox->addItem("Bay 3");
        comboBox->addItem("Bay 4");
        comboBox->addItem("Bay 5");
        comboBox->setSelectedItem("[none]");
        if(t == windowType::e){
            if(c.getSelectedBucket() >= 0){
                comboBox->setSelectedItemByIndex(selectedBucket.getBoxNum() + 1);
            }
        }
        child->add(comboBox, "bayInput");

        //BUTTON
        auto button = tgui::Button::create();
        //button->setRenderer(theme.getRenderer("Button"));
        button->setPosition(125, 200);
        button->setText("OK");
        button->setSize(100, 30);
        if(t == windowType::a){
            button->connect("pressed", [&]() {
                c.addTicket(std::dynamic_pointer_cast<tgui::EditBox>(vwindow->get("idInput"))->getText().toAnsiString(),
                    std::dynamic_pointer_cast<tgui::EditBox>(vwindow->get("vehicleInput"))->getText().toAnsiString(),
                    std::dynamic_pointer_cast<tgui::EditBox>(vwindow->get("notesInput"))->getText().toAnsiString(),
                    (std::dynamic_pointer_cast<tgui::ComboBox>(vwindow->get("bayInput"))->getSelectedItemIndex() - 1));
                status = windowStatuses::added;
            });
        }else
        if(t == windowType::e){
            button->connect("pressed", [&]() {
                selectedBucket.updateTicket(
                    std::dynamic_pointer_cast<tgui::EditBox>(vwindow->get("idInput"))->getText().toAnsiString(),
                    std::dynamic_pointer_cast<tgui::EditBox>(vwindow->get("vehicleInput"))->getText().toAnsiString(),
                    std::dynamic_pointer_cast<tgui::EditBox>(vwindow->get("notesInput"))->getText().toAnsiString());
                c.setBoxNum(c.getSelectedBucket(),
                    (std::dynamic_pointer_cast<tgui::ComboBox>(vwindow->get("bayInput"))->getSelectedItemIndex() - 1));
                status = windowStatuses::edited;
            });
        }
        child->add(button);

        //BUTTON
        button = tgui::Button::create();
        //button->setRenderer(theme.getRenderer("Button"));
        button->setPosition(275, 200);
        button->setText("Cancel");
        button->setSize(100, 30);
        button->connect("pressed", [=](){
            status = windowStatuses::base;
            child->close();
        });
        child->add(button);
    }
    catch (const tgui::Exception& e)
    {
        std::cerr << "TGUI Exception: " << e.what() << std::endl;
        //return EXIT_FAILURE;
    }
};

int main(){
    //create window and window related objects
    sf::RenderWindow window(sf::VideoMode(MAIN_WINDOW_WIDTH, MAIN_WINDOW_HEIGHT), "BayWatch 0.2.2");

    //window focus boolean
    bool hasFocus = true;

    //input pressed booleans
    bool leftMousePressed = false;

    Controller c;

    tgui::Gui gui(window);
    std::stringstream guiStream;

    try{
        auto menu = tgui::MenuBar::create();
        //menu->setRenderer(theme.getRenderer("MenuBar"));
        menu->setSize(static_cast<float>(window.getSize().x), 22.f);
        menu->addMenu("Ticket");
        menu->addMenuItem("New");
        menu->connectMenuItem({"Ticket", "New"}, [&](){
            if(status == windowStatuses::base){
                newVehicleWindow(gui, c, windowType::a);
                status = windowStatuses::add;
            }
        });
        menu->addMenuItem("Edit");
        menu->connectMenuItem({"Ticket", "Edit"}, [&](){
            if(status == windowStatuses::base){
                if(c.getSelectedBucket() >= 0){
                    newVehicleWindow(gui, c, windowType::e);
                    status = windowStatuses::edit;
                }else{
                    std::cerr << "vwindow 'Edit' build error: no bucket selected" << std::endl;
                }
            }
        });
        gui.add(menu, "menu");
    }
    catch (const tgui::Exception& e)
    {
        std::cerr << "TGUI Exception: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    /*c.addTicket("1234", "CHRYSLER", "", 2);
    c.addTicket("1235", "CHEVROLET TAHOE", "" , 3);
    c.addTicket("1235", "CHEVROLET CAPRESE", "" , 4);
    c.addTicket("1237", "CHEVROLET NEW CAPRESE", "CROSS ROTATION, CUSTOMER WAITING" , -1);

    c.removeTicket(0);
    c.addTicket("1239", "FORD AEROSTAR", "" , 3);
    c.removeTicket(0);
    c.addTicket("1240", "NISSAN ALTIMA", "SHOULD NOT APPEAR" , 3);
    c.addTicket("1241", "NISSAN CUBE", "SHOULD APPEAR" , 3);
    c.removeTicket(1);
    c.removeTicket(1);
    c.addTicket("1242", "FORD MUSTANG", "" , 3);
    c.addTicket("1243", "PONTIAC SUNFIRE", "" , 3);
    c.removeTicket(3);

    c.setBoxNum(2, 1);*/

    sf::Font freeroadR;
    freeroadR.loadFromMemory(freeroadRegular, freeroadRegular_length);
    sf::Font freeroadB;
    freeroadB.loadFromMemory(freeroadBold, freeroadBold_length);

    std::stringstream ss;

    sf::Text mx;
    mx.setFillColor(yellowish);
    mx.setFont(freeroadR);
    mx.setCharacterSize(15);
    mx.setPosition(0, 0 + TEXT_LINE_SPACING_15);

    //debugTextObjects[1]
    sf::Text my;
    my.setFillColor(darkYellowish);
    my.setFont(freeroadR);
    my.setCharacterSize(15);
    my.setPosition(0, 0 + 2*TEXT_LINE_SPACING_15);

    while(window.isOpen()){
        //EVENT OBJECT AND EVENT POLLING LOOP
        sf::Event event;
        while(window.pollEvent(event)){
            //WINDOW CLOSE EVENT, activated on pressing the close button
            if (event.type == sf::Event::Closed){
                window.close();
                return EXIT_SUCCESS;
            }

            //CHECK FOR WINDOW FOCUS BEFORE ALLOWING KEYBOARD INPUT
            if (event.type == sf::Event::LostFocus){
                hasFocus = false;
            }
            if (event.type == sf::Event::GainedFocus){
                hasFocus = true;
            }

            //GUI EVENT HANDLER
            gui.handleEvent(event);
        }//end while(window.pollEvent(event))

        //IF WINDOW HAS FOCUS, CHECK KEY INPUTS
        if(hasFocus){
            if(sf::Mouse::isButtonPressed(sf::Mouse::Left)){ //is key pressed?
                if(!leftMousePressed){ //if not pressed to start, set to pressed and do action
                    leftMousePressed = true;

                    //check to see if mouse is selecting an object
                    //BUT:
                    //  only if the mouse isn't using the menu
                    //  and only if the window status is 'base'
                    if(menubar->mouseOnWidget(tgui::Vector2f(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y)) == false
                       && status == windowStatuses::base){
                        //check if mouse is in the area of a box
                        //if yes, select that box
                        short boxIndex = c.isInBox(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y);
                        if(boxIndex != -1){
                            c.selectBox(boxIndex);
                        }else{
                            c.deselectBox();
                        }

                        //check if mouse is in the area of a bucket
                        //if yes, select that box
                        short buckIndex = c.isInBucket(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y);
                        if(buckIndex != -1){
                            c.selectBucket(buckIndex);
                        }else{
                            c.deselectBucket();
                        }
                    }

                    ss.str(std::string());
                    ss << sf::Mouse::getPosition(window).x;
                    mx.setString(ss.str());

                    ss.str(std::string());
                    ss << sf::Mouse::getPosition(window).y;
                    my.setString(ss.str());
                }
            }else{ //if the key isn't pressed, set the key's flag to false
                leftMousePressed = false;
            }

            if(status == windowStatuses::added){
                vwindow->close();

                //reset status
                status = windowStatuses::base;
            }else
            if(status == windowStatuses::edited){
                vwindow->close();

                //reset status
                status = windowStatuses::base;
            }
        }//end if(hasFocus)

        c.update(); //update controller display objects!

        //WINDOW DRAW FUNCTIONS
        window.clear(); //start by clearing the window

        //DRAW BOX OBJECTS FROM CONTROLLER
        for(unsigned short i = 0; i < c.getBoxRectObjects().size(); i++){
            window.draw(c.getBoxRectObjects().at(i));
        }
        for(unsigned short i = 0; i < c.getBoxTextObjects().size(); i++){
            window.draw(c.getBoxTextObjects().at(i));
        }
        for(unsigned short i = 0; i < c.getQueueRectObjects().size(); i++){
            window.draw(c.getQueueRectObjects().at(i));
        }
        for(unsigned short i = 0; i < c.getQueueTextObjects().size(); i++){
            window.draw(c.getQueueTextObjects().at(i));
        }

        window.draw(mx);
        window.draw(my);

        gui.draw();

        window.display(); //finally, display!
    }//end while(window.isOpen())

    return 0;
}
