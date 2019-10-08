#include <string>
#include <vector>
#include <chrono>
#include "ticket.h"
#include "rect.h"

#ifndef BOX
#define BOX

#define BOX_WARNING_TIME 900.0
#define BOX_URGENT_TIME 1800.0

static std::chrono::duration<double> fullBoxUpdateFlickeringDuration(5.8f);
static std::chrono::duration<double> singleBoxFlickerDuration(0.2f);
static std::chrono::duration<int> constructorBuffer(6); //shifts m.R.U.P. so Box does not construct already flickering

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
    std::chrono::steady_clock::time_point mostRecentUpdatePoint;
public:
    Box();
    Box(short x, short y);
    Box(short x, short y, short w, short h);

    Box(std::string newL);
    Box(std::string newL, short x, short y);
    Box(std::string newL, short x, short y, short w, short h);

    ~Box();

    void setLabel(std::string s);
    void setActive(bool b);
    void setSelected(bool b);
    void setStatus(unsigned short s);
    void setActiveStartPoint(std::chrono::steady_clock::time_point p);
    void setMostRecentUpdatePoint(std::chrono::steady_clock::time_point p);

    std::vector<Ticket*> getTickets();
    Rect* getBoundary();

    std::string getLabel();
    bool isActive();
    bool isSelected();
    unsigned short getStatus();
    std::chrono::steady_clock::time_point getActiveStartPoint();

    void addTicket(std::string newID, std::string newV, std::string newN, short s);
    void updateTicketAtIndex(unsigned short index, std::string newID, std::string newV, std::string newN);
    void removeTicket(std::string id);
    void removeTicketByIndex(unsigned short s);

    long getTimeSinceActiveAsLong(std::chrono::steady_clock::time_point p);
    double getTimeSinceActiveAsDouble(std::chrono::steady_clock::time_point p);
    std::string getTimeSinceActiveAsString(std::chrono::steady_clock::time_point p);

    bool onFlicker(std::chrono::steady_clock::time_point p);
};

#endif // BOX
