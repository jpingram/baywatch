#include <string>
#include <vector>
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
public:
    Box();
    Box(short x, short y);
    Box(short x, short y, short w, short h);

    Box(std::string newL);
    Box(std::string newL, short x, short y);
    Box(std::string newL, short x, short y, short w, short h);

    void setLabel(std::string s);
    void setActive(bool);
    void setSelected(bool);
    void setStatus(unsigned short s);

    std::vector<Ticket*> getTickets();
    Rect* getBoundary();

    std::string getLabel();
    bool isActive();
    bool isSelected();
    unsigned short getStatus();

    void addTicket(std::string newID, std::string newV, std::string newN, short s);
    void removeTicket(std::string id);
};

#endif // BOX
