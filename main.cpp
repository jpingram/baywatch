#include <SFML/Graphics.hpp>
#include <string>
#include <sstream>
#include "data.h"
#include "controller.h"

#define MAIN_WINDOW_WIDTH 1200
#define MAIN_WINDOW_HEIGHT 600

int main(){
    //create window and window related objects
    sf::RenderWindow window(sf::VideoMode(MAIN_WINDOW_WIDTH, MAIN_WINDOW_HEIGHT), "BayWatch 0.1");

    //window focus boolean
    bool hasFocus = true;

    //input pressed booleans
    bool leftMousePressed = false;

    Controller c;
    c.addTicket("1234", "CHRYSLER", "", 2);
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

    c.setBoxNum(2, 1);

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
        }//end while(window.pollEvent(event))

        //IF WINDOW HAS FOCUS, CHECK KEY INPUTS
        if(hasFocus){
            if(sf::Mouse::isButtonPressed(sf::Mouse::Left)){ //is key pressed?
                if(!leftMousePressed){ //if not pressed to start, set to pressed and do action
                    leftMousePressed = true;

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

        window.display(); //finally, display!
    }//end while(window.isOpen())

    return 0;
}
