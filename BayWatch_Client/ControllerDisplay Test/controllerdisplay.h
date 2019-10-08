#include <vector>
#include <chrono>
#include <TGUI/Gui.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include "box.h"
#include "ticketbucket.h"
#include "controller.h"

#ifndef CONTROLLER_DISPLAY
#define CONTROLLER_DISPLAY

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
#define QUEUE_NOTES_LIMIT 80

#define clock std::chrono::steady_clock

class ControllerDisplay{
private:
    tgui::Gui boxTextObjects;
    std::vector<sf::RectangleShape> boxRectObjects;
    std::vector<sf::Text> queueTextObjects;
    std::vector<sf::RectangleShape> queueRectObjects;

    clock::time_point currentTimePoint;

    sf::Font freeroadR;
    sf::Font freeroadB;
public:
    ControllerDisplay(Controller& c);

    void update(Controller& c);

    void setRenderTarget(sf::RenderTarget &target);

    tgui::Gui* getBoxTextObjects();
    std::vector<sf::RectangleShape> getBoxRectObjects();
    std::vector<sf::Text> getQueueTextObjects();
    std::vector<sf::RectangleShape> getQueueRectObjects();

    void createBoxObjects(Controller& c);
    void updateBoxObjects(Controller& c);

    void createQueueObjects(Controller& c);
    void updateQueueObjects(Controller& c);
};

#endif // CONTROLLER

