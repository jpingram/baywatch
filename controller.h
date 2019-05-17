#include <vector>
#include <TGUI/Gui.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include "box.h"
#include "ticketbucket.h"

#ifndef CONTROLLER
#define CONTROLLER

#define TEXT_LINE_SPACING_15 20.f
#define TEXT_LINE_SPACING_21 28.f
#define TEXT_LINE_SPACING_24 32.f
#define TEXT_LINE_SPACING_27 36.f
#define BOX_X_ZERO_POINT 5
#define BOX_Y_ZERO_POINT 27.f
#define BOX_WIDTH 230
#define BOX_HEIGHT 350
#define QUEUE_TICKET_LIMIT 10
#define QUEUE_X_ZERO_POINT 0
#define QUEUE_Y_ZERO_POINT 400
#define QUEUE_WIDTH 1200

enum conStatuses{noneSelected, boxSelected, bucketSelected};

class Controller{
private:
    std::vector<Box> boxList;
    std::vector<TicketBucket> ticketQueue; //MAIN QUEUE OF TICKETS; ALL TICKETS HOULD BE STORED IN MEMORY HERE

    tgui::Gui boxTextObjects;
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

    void setRenderTarget(sf::RenderTarget &target);

    std::vector<Box>* getBoxList();
    std::vector<TicketBucket>* getTicketQueue();

    tgui::Gui* getBoxTextObjects();
    std::vector<sf::RectangleShape> getBoxRectObjects();
    std::vector<sf::Text> getQueueTextObjects();
    std::vector<sf::RectangleShape> getQueueRectObjects();

    bool getStatus();
    void setStatus(unsigned short s);

    short isInBox(short x, short y); //if point is in box, returns index of box in boxList; else, returns -1
    void selectBox(short s);
    void deselectBox();
    short getSelectedBox();

    short isInBucket(short x, short y); //if point is in a bucket, returns index of bucket in ticketQueue; else, returns -1
    void selectBucket(short s);
    void deselectBucket();
    short getSelectedBucket();

    short addTicket(std::string newID, std::string newV, std::string newN, short s); //returns 0 is add successful, -1 if failed
    bool isIdUnique(std::string id);
    void setBoxNum(unsigned short index, short newBox);
    void updateTicket(short index, std::string newID, std::string newV, std::string newN); //updates ticketbucket at index and copy of ticket in box
    void removeTicket(unsigned short s);

    void update();

    void createBoxObjects();
    void updateBoxObjects();

    void createQueueObjects();
    void updateQueueObjects();
};

#endif // CONTROLLER
