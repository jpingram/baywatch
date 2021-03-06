#include <TGUI/TGUI.hpp>
#include <SFML/Graphics.hpp>
#include <string>
#include <sstream>
#include <iostream>
#include <memory>
#include "data.h"
#include "controller.h"
#include "ticket.h"

#define MAIN_WINDOW_WIDTH 1200
#define MAIN_WINDOW_HEIGHT 600

#define vwindow std::dynamic_pointer_cast<tgui::ChildWindow>(gui.get("child"))
#define menubar std::dynamic_pointer_cast<tgui::MenuBar>(gui.get("menu"))
#define selectedBox c.getBoxList()->at(c.getSelectedBox())
#define selectedBucket c.getTicketQueue()->at(c.getSelectedBucket())
#define MAX_DISPLAY_WINDOWS 2

enum windowStatuses{base, add, added, edit, bedit, edited, cancelled, closed};
enum windowType{a, e};
static unsigned short status = windowStatuses::base;
static unsigned short numDisplayWindows = 0;
//static bool bayDisplayActive = false;

void newVehicleWindow(tgui::Gui& gui, Controller& c, unsigned short t){
    try{
        //CHILDWINDOW
        auto child = tgui::ChildWindow::create();
        child->setSize(500, 300);
        child->setPosition(200, 200);
        if(t == windowType::a){
            child->setTitle("New Ticket Info");
        }else
        if(t == windowType::e){
            child->setTitle("Edit Ticket Info");
        }
        child->connect("closed", [&](){
            status = windowStatuses::closed;
        });
        gui.add(child, "child");

        //LABEL
        auto label = tgui::Label::create();
        label->setText("Ticket ID:");
        label->setPosition(10, 30);
        label->setTextSize(15);
        child->add(label, "idLabel");

        //EDITBOX
        auto editBox = tgui::EditBox::create();
        editBox->setSize(365, 25);
        editBox->setTextSize(18);
        editBox->setPosition(125, 30);
        if(t == windowType::e){
            if(c.getSelectedBucket() >= 0){
                editBox->setText(selectedBucket.getTicket()->getID());
            }else{
                editBox->setDefaultText("ERROR: invalid selection");
            }
        }else{
            editBox->setDefaultText("Ex: 1234");
        }
        child->add(editBox, "idInput");

        //LABEL
        label = tgui::Label::create();
        label->setText("Vehicle Info:");
        label->setPosition(10, 60);
        label->setTextSize(15);
        child->add(label, "vehicleLabel");

        //EDITBOX
        editBox = tgui::EditBox::create();
        editBox->setSize(365, 25);
        editBox->setTextSize(18);
        editBox->setPosition(125, 60);
        if(t == windowType::e){
            if(c.getSelectedBucket() >= 0){
                editBox->setText(selectedBucket.getTicket()->getVehicle());
            }else{
                editBox->setDefaultText("ERROR: invalid selection");
            }
        }else{
            editBox->setDefaultText("Ex: 2001 Chrysler Sebring");
        }
        child->add(editBox, "vehicleInput");

        //LABEL
        label = tgui::Label::create();
        label->setText("Notes:");
        label->setPosition(10, 90);
        label->setTextSize(15);
        child->add(label, "notesLabel");

        //EDITBOX
        editBox = tgui::EditBox::create();
        editBox->setSize(365, 25);
        editBox->setTextSize(18);
        editBox->setPosition(125, 90);
        if(t == windowType::e){
            if(c.getSelectedBucket() >= 0){
                editBox->setText(selectedBucket.getTicket()->getNotes());
            }else{
                editBox->setDefaultText("ERROR: invalid selection");
            }
        }else{
            editBox->setDefaultText("Ex: needs an oil change.");
        }
        child->add(editBox, "notesInput");

        //LABEL
        label = tgui::Label::create();
        label->setText("Bay:");
        label->setPosition(10, 120);
        label->setTextSize(15);
        child->add(label, "bayLabel");

        //combobox for now
        auto comboBox = tgui::ComboBox::create();
        comboBox->setSize(120, 30);
        comboBox->setPosition(50, 120);
        comboBox->addItem("[none]");
        for(unsigned short i = 0; i < c.getBoxList()->size(); i++){
            comboBox->addItem(c.getBoxList()->at(i).getLabel());
        }
        comboBox->setSelectedItem("[none]");
        if(c.getSelectedBox() >= 0){
            comboBox->setSelectedItemByIndex(c.getSelectedBox() + 1);
        }
        if(t == windowType::e){
            if(c.getSelectedBucket() >= 0){
                comboBox->setSelectedItemByIndex(selectedBucket.getBoxNum() + 1);
            }
        }
        child->add(comboBox, "bayInput");

        //LABEL
        label = tgui::Label::create();
        label->setText("");
        label->setPosition(10, 175);
        label->setTextSize(12);
        label->getRenderer()->setTextColor(tgui::Color(sf::Color::Red));
        child->add(label, "warningLabel");

        //BUTTON
        auto button = tgui::Button::create();
        button->setPosition(125, 200);
        button->setText("OK");
        button->setSize(100, 30);
        if(t == windowType::a){
            button->connect("pressed", [&]() {
                std::stringstream ss; //used to construct error messages
                ss.str(std::string());

                if(std::dynamic_pointer_cast<tgui::EditBox>(vwindow->get("idInput"))->getText().toAnsiString().compare("") == 0){
                    std::dynamic_pointer_cast<tgui::Label>(vwindow->get("warningLabel"))
                        ->setText("Error: \"Ticket ID\" cannot be left blank.");
                }else
                if(c.getBucketIndexById(std::dynamic_pointer_cast<tgui::EditBox>(vwindow->get("idInput"))
                                    ->getText().toAnsiString()) != -1){
                    std::dynamic_pointer_cast<tgui::Label>(vwindow->get("warningLabel"))
                        ->setText("Error: \"Ticket ID\" is already being used.");
                }else
                if(std::dynamic_pointer_cast<tgui::EditBox>(vwindow->get("vehicleInput"))->getText().toAnsiString()
                                    .size() > VEHICLE_CHAR_LIMIT){
                    ss << "Error: \"Vehicle Info\" can not be more than " << VEHICLE_CHAR_LIMIT << " characters.";
                    std::dynamic_pointer_cast<tgui::Label>(vwindow->get("warningLabel"))->setText(ss.str());
                }else
                if(std::dynamic_pointer_cast<tgui::EditBox>(vwindow->get("notesInput"))->getText().toAnsiString()
                                    .size() > NOTES_CHAR_LIMIT){
                    ss << "Error: \"Notes\" can not be more than " << NOTES_CHAR_LIMIT << " characters.";
                    std::dynamic_pointer_cast<tgui::Label>(vwindow->get("warningLabel"))->setText(ss.str());
                }else{
                    c.addTicket(std::dynamic_pointer_cast<tgui::EditBox>(vwindow->get("idInput"))->getText(),
                        std::dynamic_pointer_cast<tgui::EditBox>(vwindow->get("vehicleInput"))->getText().toAnsiString(),
                        std::dynamic_pointer_cast<tgui::EditBox>(vwindow->get("notesInput"))->getText().toAnsiString(),
                        (std::dynamic_pointer_cast<tgui::ComboBox>(vwindow->get("bayInput"))->getSelectedItemIndex() - 1));
                    status = windowStatuses::added;
                }
            });
        }else
        if(t == windowType::e){
            button->connect("pressed", [&]() {
                std::stringstream ss; //used to construct error messages
                ss.str(std::string());

                if(std::dynamic_pointer_cast<tgui::EditBox>(vwindow->get("idInput"))->getText().toAnsiString().compare("") == 0){
                    std::dynamic_pointer_cast<tgui::Label>(vwindow->get("warningLabel"))
                        ->setText("Error: \"Ticket ID\" cannot be left blank.");
                }else
                if(c.getBucketIndexById(std::dynamic_pointer_cast<tgui::EditBox>(vwindow->get("idInput"))
                                    ->getText().toAnsiString()) != -1 &&
                   c.getBucketIndexById(std::dynamic_pointer_cast<tgui::EditBox>(vwindow->get("idInput"))
                                    ->getText().toAnsiString()) != c.getSelectedBucket()){
                    std::dynamic_pointer_cast<tgui::Label>(vwindow->get("warningLabel"))
                        ->setText("Error: \"Ticket ID\" is being used by another ticket.");
                }else
                if(std::dynamic_pointer_cast<tgui::EditBox>(vwindow->get("vehicleInput"))->getText().toAnsiString()
                                    .size() > VEHICLE_CHAR_LIMIT){
                    ss << "Error: \"Vehicle Info\" can not be more than " << VEHICLE_CHAR_LIMIT << " characters.";
                    std::dynamic_pointer_cast<tgui::Label>(vwindow->get("warningLabel"))->setText(ss.str());
                }else
                if(std::dynamic_pointer_cast<tgui::EditBox>(vwindow->get("notesInput"))->getText().toAnsiString()
                                    .size() > NOTES_CHAR_LIMIT){
                    ss << "Error: \"Notes\" can not be more than " << NOTES_CHAR_LIMIT << " characters.";
                    std::dynamic_pointer_cast<tgui::Label>(vwindow->get("warningLabel"))->setText(ss.str());
                }else{
                    //NOTE: ORDER OF OERATIONS: box change relies on previous vehicle info
                    //  therefore, box change must happen first
                    //UPDATE: order doesn't matter with addition of updateTicket function in Controller
                    c.setBoxNum(c.getSelectedBucket(),
                        (std::dynamic_pointer_cast<tgui::ComboBox>(vwindow->get("bayInput"))->getSelectedItemIndex() - 1));
                    c.updateTicket(
                        c.getSelectedBucket(),
                        std::dynamic_pointer_cast<tgui::EditBox>(vwindow->get("idInput"))->getText().toAnsiString(),
                        std::dynamic_pointer_cast<tgui::EditBox>(vwindow->get("vehicleInput"))->getText().toAnsiString(),
                        std::dynamic_pointer_cast<tgui::EditBox>(vwindow->get("notesInput"))->getText().toAnsiString());
                    status = windowStatuses::edited;
                }
            });
        }
        child->add(button);

        //BUTTON
        button = tgui::Button::create();
        button->setPosition(275, 200);
        button->setText("Cancel");
        button->setSize(100, 30);
        button->connect("pressed", [=](){
            status = windowStatuses::closed;
        });
        child->add(button);
    }
    catch (const tgui::Exception& e)
    {
        std::cerr << "TGUI Exception in newVehicleWindow(): " << e.what() << std::endl;
        //return EXIT_FAILURE;
    }
};

void newBayWindow(tgui::Gui& gui, Controller& c){
    try{
        //CHILDWINDOW
        auto child = tgui::ChildWindow::create();
        child->setSize(500, 100);
        child->setPosition(200, 200);
        child->setTitle("Edit Bay Info");
        child->connect("closed", [&](){
            status = windowStatuses::closed;
        });
        gui.add(child, "child");

        //LABEL
        auto label = tgui::Label::create();
        label->setText("Bay Label:");
        label->setPosition(10, 30);
        label->setTextSize(15);
        child->add(label, "idLabel");

        //EDITBOX
        auto editBox = tgui::EditBox::create();
        editBox->setSize(365, 25);
        editBox->setTextSize(18);
        editBox->setPosition(125, 30);
        if(c.getSelectedBox() >= 0){
            editBox->setText(selectedBox.getLabel());
        }else{
            editBox->setDefaultText("ERROR: invalid selection");
        }
        child->add(editBox, "idInput");

        //BUTTON
        auto button = tgui::Button::create();
        button->setPosition(125, 60);
        button->setText("OK");
        button->setSize(100, 30);
        button->connect("pressed", [&]() {
            selectedBox.setLabel(std::dynamic_pointer_cast<tgui::EditBox>(vwindow->get("idInput"))->getText().toAnsiString());
            status = windowStatuses::edited;
        });
        child->add(button);

        //BUTTON
        button = tgui::Button::create();
        button->setPosition(275, 60);
        button->setText("Cancel");
        button->setSize(100, 30);
        button->connect("pressed", [=](){
            status = windowStatuses::base;
            child->destroy();
        });
        child->add(button);
    }
    catch (const tgui::Exception& e)
    {
        std::cerr << "TGUI Exception in newBayWindow(): " << e.what() << std::endl;
        //return EXIT_FAILURE;
    }
};

void openBayDisplayWindow(sf::RenderWindow &target, tgui::Gui &gui){
    target.create(sf::VideoMode(MAIN_WINDOW_WIDTH, MAIN_WINDOW_HEIGHT), "BayWatch 0.5.2 Bay Display");
    sf::Image icon;
    icon.loadFromMemory(logo32, logo32_length);
    target.setIcon(32, 32, icon.getPixelsPtr());

    numDisplayWindows++;

    if(numDisplayWindows == MAX_DISPLAY_WINDOWS){
        menubar->setMenuItemEnabled("Window", "Show Bay Display Window", false);
    }else{
        menubar->setMenuItemEnabled("Window", "Show Bay Display Window", true);
    }
    menubar->setMenuItemEnabled("Window", "Close Bay Display Window", true);
}

void closeBayDisplayWindow(sf::RenderWindow &target, tgui::Gui &gui){
    target.close();

    numDisplayWindows--;

    menubar->setMenuItemEnabled("Window", "Show Bay Display Window", true);
    if(numDisplayWindows == 0){
        menubar->setMenuItemEnabled("Window", "Close Bay Display Window", false);
    }else{
        menubar->setMenuItemEnabled("Window", "Close Bay Display Window", true);
    }
}

int main(){
    //create window and window related objects
    sf::RenderWindow window(sf::VideoMode(MAIN_WINDOW_WIDTH, MAIN_WINDOW_HEIGHT), "BayWatch 0.5.2");
    sf::Image icon;
    icon.loadFromMemory(logo32, logo32_length);
    window.setIcon(32, 32, icon.getPixelsPtr());

    //display windows (instantiate but don't construct)
    sf::RenderWindow window2;
    sf::RenderWindow window3;

    //window focus boolean
    bool hasFocus = true;

    //input pressed booleans
    bool leftMousePressed = false;

    Controller c;
    c.setRenderTarget(window);

    tgui::Gui gui(window);
    std::stringstream guiStream;

    try{
        auto menu = tgui::MenuBar::create();
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
                }
                if(c.getSelectedBox() >= 0){
                    if(!selectedBox.getTickets().empty()){
                        //set the selected bucket
                        c.selectBucket(c.getBucketIndexById(selectedBox.getTickets().at(0)->getID()));
                        c.deselectBox();

                        //call the vwindow like with a regularly selected bucket
                        newVehicleWindow(gui, c, windowType::e);
                        status = windowStatuses::edit;
                    }else{
                        std::cerr << "vwindow 'Edit' build error: no bucket selected" << std::endl;
                    }
                }else{
                    std::cerr << "vwindow 'Edit' build error: no bucket selected" << std::endl;
                }
            }
        });

        menu->addMenuItem("Remove");
        menu->connectMenuItem({"Ticket", "Remove"}, [&](){
            if(status == windowStatuses::base){
                if(c.getSelectedBucket() >= 0){
                    c.removeTicket(c.getSelectedBucket());
                }else{
                    std::cerr << "'Remove' error: no bucket selected" << std::endl;
                }
            }
        });


        menu->addMenu("Bay");

        menu->addMenuItem("Edit");
        menu->connectMenuItem({"Bay", "Edit"}, [&](){
            if(status == windowStatuses::base){
                if(c.getSelectedBox() >= 0){
                    newBayWindow(gui, c);
                    status = windowStatuses::bedit;
                }else{
                    std::cerr << "bwindow 'Edit' build error: no bay selected" << std::endl;
                }
            }
        });

        menu->addMenuItem("Clear Active Ticket");
        menu->connectMenuItem({"Bay", "Clear Active Ticket"}, [&](){
            if(status == windowStatuses::base){
                if(c.getSelectedBox() >= 0){
                    c.removeActiveTicketFromBox(c.getSelectedBox());
                }else{
                    std::cerr << "'Clear Active Ticket' error: no bay selected" << std::endl;
                }
            }
        });

        menu->addMenuItem("Clear All Tickets");
        menu->connectMenuItem({"Bay", "Clear All Tickets"}, [&](){
            if(status == windowStatuses::base){
                if(c.getSelectedBox() >= 0){
                    c.removeAllTicketsFromBox(c.getSelectedBox());
                }else{
                    std::cerr << "'Clear All Tickets' error: no bay selected" << std::endl;
                }
            }
        });

        menu->addMenu("Window");
        menu->addMenuItem("Show Bay Display Window");
        menu->connectMenuItem({"Window", "Show Bay Display Window"}, [&](){
            if(!window2.isOpen()){
                openBayDisplayWindow(window2, gui);
            }else
            if(!window3.isOpen()){
                openBayDisplayWindow(window3, gui);
            }
        });

        menu->addMenuItem("Close Bay Display Window");
        menu->connectMenuItem({"Window", "Close Bay Display Window"}, [&](){
            if(window3.isOpen()){
                closeBayDisplayWindow(window3, gui);
            }else
            if(window2.isOpen()){
                closeBayDisplayWindow(window2, gui);
            }
        });

        menu->setMenuItemEnabled("Window", "Show Bay Display Window", true);
        menu->setMenuItemEnabled("Window", "Close Bay Display Window", false);

        gui.add(menu, "menu");
    }
    catch (const tgui::Exception& e)
    {
        std::cerr << "TGUI Exception: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    sf::Font freeroadR;
    freeroadR.loadFromMemory(freeroadRegular, freeroadRegular_length);

    sf::Font freeroadB;
    freeroadB.loadFromMemory(freeroadBold, freeroadBold_length);

    std::stringstream ss;

    sf::Text mx;
    mx.setFillColor(yellowish);
    mx.setFont(freeroadR);
    mx.setCharacterSize(15);
    mx.setPosition(MAIN_WINDOW_WIDTH - 50, MAIN_WINDOW_HEIGHT - 4*TEXT_LINE_SPACING_15);

    sf::Text my;
    my.setFillColor(darkYellowish);
    my.setFont(freeroadR);
    my.setCharacterSize(15);
    my.setPosition(MAIN_WINDOW_WIDTH - 50, MAIN_WINDOW_HEIGHT - 3*TEXT_LINE_SPACING_15);

    sf::Text mx2;
    mx2.setFillColor(greenish);
    mx2.setFont(freeroadR);
    mx2.setCharacterSize(15);
    mx2.setPosition(MAIN_WINDOW_WIDTH - 50, MAIN_WINDOW_HEIGHT - 2*TEXT_LINE_SPACING_15);

    sf::Text my2;
    my2.setFillColor(darkGreenish);
    my2.setFont(freeroadR);
    my2.setCharacterSize(15);
    my2.setPosition(MAIN_WINDOW_WIDTH - 50, MAIN_WINDOW_HEIGHT - TEXT_LINE_SPACING_15);

    while(window.isOpen()){
        //EVENT OBJECT AND EVENT POLLING LOOP
        sf::Event event;
        while(window.pollEvent(event)){
            //WINDOW CLOSE EVENT, activated on pressing the close button
            if (event.type == sf::Event::Closed){
                if(window2.isOpen()){
                    closeBayDisplayWindow(window2, gui);
                }
                if(window3.isOpen()){
                    closeBayDisplayWindow(window3, gui);
                }
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

        if(window2.isOpen()){
            //EVENT OBJECT AND EVENT POLLING LOOP
            sf::Event event;
            while(window2.pollEvent(event)){
                //WINDOW CLOSE EVENT, activated on pressing the close button
                if (event.type == sf::Event::Closed){
                    closeBayDisplayWindow(window2, gui);
                }
            }
        }//end while(window2.pollEvent(event))

        if(window3.isOpen()){
            //EVENT OBJECT AND EVENT POLLING LOOP
            sf::Event event;
            while(window3.pollEvent(event)){
                //WINDOW CLOSE EVENT, activated on pressing the close button
                if (event.type == sf::Event::Closed){
                    closeBayDisplayWindow(window3, gui);
                }
            }
        }//end while(window3.pollEvent(event))

        //IF WINDOW HAS FOCUS, CHECK KEY INPUTS
        if(hasFocus){
            if(sf::Mouse::isButtonPressed(sf::Mouse::Left)){ //is key pressed?
                if(!leftMousePressed){ //if not pressed to start, set to pressed and do action
                    leftMousePressed = true;

                    sf::Vector2i realMouseLoc = sf::Mouse::getPosition(window);
                    sf::Vector2f relativeMouseLoc = window.mapPixelToCoords(realMouseLoc);
                    //check to see if mouse is selecting an object
                    //BUT:
                    //  only if the mouse isn't using the menu
                    //  and only if the window status is 'base'
                    if(menubar->mouseOnWidget(tgui::Vector2f(realMouseLoc.x, realMouseLoc.y)) == false
                       && status == windowStatuses::base){
                        //check if mouse is in the area of a box
                        //if yes, select that box
                        short boxIndex = c.isInBox(relativeMouseLoc.x, relativeMouseLoc.y);
                        if(boxIndex != -1){
                            c.selectBox(boxIndex);
                        }else{
                            c.deselectBox();
                        }

                        //check if mouse is in the area of a bucket
                        //if yes, select that box
                        short buckIndex = c.isInBucket(relativeMouseLoc.x, relativeMouseLoc.y);
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

                    ss.str(std::string());
                    ss << relativeMouseLoc.x;
                    mx2.setString(ss.str());

                    ss.str(std::string());
                    ss << relativeMouseLoc.y;
                    my2.setString(ss.str());
                }
            }else{ //if the key isn't pressed, set the key's flag to false
                leftMousePressed = false;
            }

            if(status == windowStatuses::added){
                vwindow->destroy();

                //reset status
                status = windowStatuses::base;
            }else
            if(status == windowStatuses::edited){
                vwindow->destroy();

                //reset status
                status = windowStatuses::base;
            }else
            if(status == windowStatuses::closed){
                vwindow->destroy();

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
        /*for(unsigned short i = 0; i < c.getBoxTextObjects().size(); i++){
            window.draw(c.getBoxTextObjects().at(i));
        }*/
        c.setRenderTarget(window);
        c.getBoxTextObjects()->draw();
        for(unsigned short i = 0; i < c.getQueueRectObjects().size(); i++){
            window.draw(c.getQueueRectObjects().at(i));
        }
        for(unsigned short i = 0; i < c.getQueueTextObjects().size(); i++){
            window.draw(c.getQueueTextObjects().at(i));
        }

        if(window2.isOpen()){
            for(unsigned short i = 0; i < c.getBoxRectObjects().size(); i++){
                window2.draw(c.getBoxRectObjects().at(i));
            }
            c.setRenderTarget(window2);
            c.getBoxTextObjects()->draw();
            for(unsigned short i = 0; i < c.getQueueRectObjects().size(); i++){
                window2.draw(c.getQueueRectObjects().at(i));
            }
            for(unsigned short i = 0; i < c.getQueueTextObjects().size(); i++){
                window2.draw(c.getQueueTextObjects().at(i));
            }

            window2.display();
        }

        if(window3.isOpen()){
            for(unsigned short i = 0; i < c.getBoxRectObjects().size(); i++){
                window3.draw(c.getBoxRectObjects().at(i));
            }
            c.setRenderTarget(window3);
            c.getBoxTextObjects()->draw();
            for(unsigned short i = 0; i < c.getQueueRectObjects().size(); i++){
                window3.draw(c.getQueueRectObjects().at(i));
            }
            for(unsigned short i = 0; i < c.getQueueTextObjects().size(); i++){
                window3.draw(c.getQueueTextObjects().at(i));
            }

            window3.display();
        }

        //KEPT FOR DEBUG/VISUAL REFERENCE PURPOSES
        //window.draw(mx);
        //window.draw(my);
        //window.draw(mx2);
        //window.draw(my2);

        gui.draw();

        window.display(); //finally, display!
    }//end while(window.isOpen())

    return EXIT_SUCCESS;
}
