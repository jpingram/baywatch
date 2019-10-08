#include <string>

#ifndef TICKET
#define TICKET

#define VEHICLE_CHAR_LIMIT 40
#define NOTES_CHAR_LIMIT 125

class Ticket{
private:
    std::string id;
    std::string vehicle;
    std::string notes;
public:
    Ticket();
    Ticket(std::string newID);
    Ticket(std::string newID, std::string newV);
    Ticket(std::string newID, std::string newV, std::string newN);

    void setID(std::string newID);
    void setVehicle(std::string newV);
    void setNotes(std::string newN);

    std::string getID();
    std::string getVehicle();
    std::string getNotes();
};

#endif // TICKET
