#ifndef __H_FOLDER
#define __H_FOLDER
#include <set>
#include "common.h"
#include "message.h"
class Folder
{
    public:
        explicit Folder(std::string _name="") : name(_name) { }
        void remMsg(const Message *);
        void addMsg(const Message *);
        const std::string &getName() { return name; }
    private:
        std::string name;
        std::set<const Message*> messages; 
};
#endif