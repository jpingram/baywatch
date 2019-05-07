#include <string>
#include "ticket.h"

#ifndef TICKETBUCKET
#define TICKETBUCKET

class TicketBucket{
private:
    Ticket ticket;
    short boxNum;

    bool selected;
public:
    TicketBucket();
    TicketBucket(std::string newID, std::string newV, std::string newN);
    TicketBucket(std::string newID, std::string newV, std::string newN, short s);

    void updateTicket(std::string newID, std::string newV, std::string newN);
    void setBoxNum(short s);
    void setSelected(bool b);

    Ticket* getTicket();
    short getBoxNum(); //denotes bay ticket is assigned (-1 if no bay currently assigned)
    bool isSelected();
};

#endif // TICKETBUCKET
