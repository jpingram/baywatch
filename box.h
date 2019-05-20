#include <string>
#include <vector>
#include <chrono>
#include "ticket.h"
#include "rect.h"

#ifndef BOX
#define BOX

enum selectStatuses{standby, stage1, stage2, stage3};

class Box{
private:
    std::string label;
    std::vector<Ticket*> tickets;

    Rect boundary;
    bool active;
    bool selected;
    unsigned short status;

    std::chrono::steady_clock::time_point activeStartPoint;
public:
    Box();
    Box(short x, short y);
    Box(short x, short y, short w, short h);

    Box(std::string newL);
    Box(std::string newL, short x, short y);
    Box(std::string newL, short x, short y, short w, short h);

    void setLabel(std::string s);
    void setActive(bool b);
    void setSelected(bool b);
    void setStatus(unsigned short s);
    void setActiveStartPoint(std::chrono::steady_clock::time_point p);

    std::vector<Ticket*> getTickets();
    Rect* getBoundary();

    std::string getLabel();
    bool isActive();
    bool isSelected();
    unsigned short getStatus();
    std::chrono::steady_clock::time_point getActiveStartPoint();

    void addTicket(std::string newID, std::string newV, std::string newN, short s);
    void removeTicket(std::string id);
    void removeTicketByIndex(unsigned short s);

    std::string getTimeSinceActiveAsString(std::chrono::steady_clock::time_point p);
};

#endif // BOX
