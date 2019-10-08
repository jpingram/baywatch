#include <string>
#include <chrono>
#include "ticket.h"

#ifndef TICKETBUCKET
#define TICKETBUCKET

static std::chrono::duration<double> fullUpdateFlickeringDuration(5.8f);
static std::chrono::duration<double> singleFlickerDuration(0.2f);

class TicketBucket{
private:
    Ticket ticket;
    short boxNum;

    bool selected;
    bool active;

    std::chrono::steady_clock::time_point birthPoint;
    std::chrono::steady_clock::time_point mostRecentUpdatePoint;
public:
    TicketBucket();
    TicketBucket(std::string newID, std::string newV, std::string newN);
    TicketBucket(std::string newID, std::string newV, std::string newN, short s);

    void updateTicket(std::string newID, std::string newV, std::string newN);
    void setBoxNum(short s);
    void setSelected(bool b);
    void setActive(bool b);
    void setBirthPoint(std::chrono::steady_clock::time_point p);
    void setMostRecentUpdatePoint(std::chrono::steady_clock::time_point p);

    Ticket* getTicket();
    short getBoxNum(); //denotes bay ticket is assigned (-1 if no bay currently assigned)
    bool isSelected();
    bool isActive();

    std::chrono::duration<long> getTimeSinceBirth(std::chrono::steady_clock::time_point p);
    std::string getTimeSinceBirthAsString(std::chrono::steady_clock::time_point p);

    bool onFlicker(std::chrono::steady_clock::time_point p);
};

#endif // TICKETBUCKET
