#ifndef __H_MESSAGE
#define __H_MESSAGE
#include <iostream>
#include <string>
#include <set>
#include "common.h"
#include "folder.h"
class Message
{
    friend class Folder;
    friend std::ostream &operator<<(std::ostream &, const Message &);
    public:
        explicit Message(const std::string &str="") :
            contents(str) {}
        Message(const Message&);
        Message &operator=(const Message&);
        ~Message();
        void save(Folder&);
        void remove(Folder&);
    private:
        std::string contents;
        std::set<Folder*> folders;
        void add_to_folders(const Message&);
        void remove_from_folders();
};
#endif