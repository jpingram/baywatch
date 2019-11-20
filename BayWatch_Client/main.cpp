#include <windows.h>
#include <iphlpapi.h>
#include <TGUI/TGUI.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <string>
#include <sstream>
#include <iostream>
#include <memory>
#include <vector>
#include <map>
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
//static unsigned short status = windowStatuses::base;
//static unsigned short numDisplayWindows = 0;

void recievePacket(sf::Packet &packet, Controller &c){
    std::stringstream ps;
    std::string test;
    sf::Uint8 clicks;
    packet >> test >> clicks;
    ps << "Packet text: ";
    ps << test;
    ps << (int)clicks << std::endl;

    sf::Uint16 bucketCount;
    sf::Uint16 boxCount;
    packet >> bucketCount >> boxCount;
    ps << bucketCount << " " << boxCount;
    std::map<std::string, int> idTable;
    for(unsigned short i = 0; i < bucketCount; i++){
        std::string id;
        std::string vehicle;
        std::string notes;
        sf::Int16 boxNum;
        sf::Int64 ticks;
        packet >> id >> vehicle >> notes >> boxNum >> ticks;
        std::chrono::duration<long> birthpointDur(ticks);
        ps << std::endl << boxNum << " " << id << std::endl << vehicle << std::endl << notes << std::endl;

        try{
            if(c.isIdUnique(id)){
                c.addTicket(id, vehicle, notes, boxNum);
                std::chrono::steady_clock::time_point newbp(std::chrono::steady_clock::now());
                newbp -= birthpointDur;
                c.getTicketQueue()->at(c.getBucketIndexById(id)).setBirthPoint(newbp);
            }else{
                c.setBoxNum(c.getBucketIndexById(id), boxNum);
                c.updateTicket(c.getBucketIndexById(id), id, vehicle, notes);
            }
            idTable[id]++;
        }catch(std::exception &e){
            std::cout << e.what() << std::endl;
        }
    }

    try{
        unsigned short i = 0;
        std::vector<TicketBucket>* tickets = c.getTicketQueue();
        while(i < tickets->size()){
            if(idTable.count(tickets->at(i).getTicket()->getID()) == 0){
                c.removeTicket(i);
            }else{
                i++;
            }
        }

        c.sortTickets();
    }catch(std::exception &e){
        std::cout << e.what() << std::endl;
    }

    ps << std::endl;
    for(unsigned short i = 0; i < boxCount; i++){
        std::string label;
        sf::Int64 ticks;
        packet >> label >> ticks;
        ps << label << std::endl;

        try{
            c.updateBox(i, label, ticks);
        }catch(std::exception &e){
            std::cout << e.what() << std::endl;
        }
    }

    std::cout << ps.str() << std::endl;
}

int main(){
    //create window and window related objects
    sf::RenderWindow window(sf::VideoMode(MAIN_WINDOW_WIDTH, MAIN_WINDOW_HEIGHT), "BayWatch 0.6.2 Client");
    sf::Image icon;
    icon.loadFromMemory(logo32, logo32_length);
    window.setIcon(32, 32, icon.getPixelsPtr());

    //display windows (instantiate but don't construct)
    //sf::RenderWindow window2;
    //sf::RenderWindow window3;

    //window focus boolean
    bool hasFocus = true;

    //input pressed booleans
    //bool leftMousePressed = false;

    Controller c;
    c.setRenderTarget(window);

    tgui::Gui gui(window);

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

    unsigned short port = 62208;
    sf::UdpSocket socket;
    socket.bind(port);
    socket.setBlocking(false);
    sf::IpAddress address;

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
        }//end if(hasFocus)

        PMIB_IPADDRTABLE pIPAddrTable;
        DWORD tableSize = 0;
        GetIpAddrTable(NULL, &tableSize, 0);
        if (tableSize > 0)
        {
            pIPAddrTable = (PMIB_IPADDRTABLE)calloc(tableSize, 1);
            if (GetIpAddrTable(pIPAddrTable, &tableSize, 0) == NO_ERROR)
            {
                for(unsigned int n = 0; n < pIPAddrTable->dwNumEntries; n++)
                {
                    sf::IpAddress ip(ntohl((pIPAddrTable->table[n].dwAddr
                                            & pIPAddrTable->table[n].dwMask)
                                           | ~pIPAddrTable->table[n].dwMask));
                    sf::Packet packet;
                    if(socket.receive(packet, ip, port) == sf::Socket::Done){
                        recievePacket(packet, c);
                    }
                }
            }
            free(pIPAddrTable);
        }

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

        //KEPT FOR DEBUG/VISUAL REFERENCE PURPOSES
        //window.draw(mx);
        //window.draw(my);
        //window.draw(mx2);
        //window.draw(my2);

        //gui.draw();

        window.display(); //finally, display!
    }//end while(window.isOpen())

    return EXIT_SUCCESS;
}
