#include <vector>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include "box.h"
#include "ticketbucket.h"

#ifndef CONTROLLER
#define CONTROLLER

#define TEXT_LINE_SPACING_15 20.f
#define QUEUE_TICKET_LIMIT 10
#define QUEUE_X_ZERO_POINT 0
#define QUEUE_Y_ZERO_POINT 400
#define QUEUE_WIDTH 1200

//enum conStatuses{boxselected, standby};

class Controller{
private:
    std::vector<Box> boxList;
    std::vector<TicketBucket> ticketQueue; //MAIN QUEUE OF TICKETS; ALL TICKETS HOULD BE STORED IN MEMORY HERE

    std::vector<sf::Text> boxTextObjects;
    std::vector<sf::RectangleShape> boxRectObjects;
    std::vector<sf::Text> queueTextObjects;
    std::vector<sf::RectangleShape> queueRectObjects;

    unsigned short status;
    short selectedBox; //index of selected box in boxList vector, -1 if no box selected
    short selectedBucket;

    sf::Font freeroadR;
    sf::Font freeroadB;
public:
    Controller();

    std::vector<sf::Text> getBoxTextObjects();
    std::vector<sf::RectangleShape> getBoxRectObjects();
    std::vector<sf::Text> getQueueTextObjects();
    std::vector<sf::RectangleShape> getQueueRectObjects();

    bool getStatus();
    void setStatus(unsigned short s);

    short isInBox(short x, short y); //if point is in box, returns index of box in boxList; else, returns -1
    void selectBox(short s);
    void deselectBox();

    short isInBucket(short x, short y); //if point is in a bucket, returns index of bucket in ticketQueue; else, returns -1
    void selectBucket(short s);
    void deselectBucket();

    short addTicket(std::string newID, std::string newV, std::string newN, short s); //returns 0 is add successful, -1 if failed
    bool isIdUnique(std::string id);
    void setBoxNum(unsigned short index, short newBox);
    void removeTicket(unsigned short s);

    void update();

    void createBoxObjects();
    void updateBoxObjects();

    void createQueueObjects();
    void updateQueueObjects();
};

#endif // CONTROLLER
