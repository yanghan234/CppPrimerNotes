#include <iostream>
#include "message.h"
#include "folder.h"

// copy constructor
Message::Message(const Message &m)
{
    this->contents = m.contents;
    this->folders = m.folders;
    add_to_folders(*this);
}

// copy-assignment constructor
Message &Message::operator=(const Message &rhs )
{
    remove_from_folders();
    this->contents = rhs.contents;
    this->folders = rhs.folders;
    add_to_folders(rhs);
    return *this;
}

// destructor
Message::~Message()
{
    this->remove_from_folders();
}

// remove from all folders
void Message::remove_from_folders()
{
    for ( Folder *f: folders )
        f->remMsg(this);
}

// remove one folder
void Message::remove(Folder &folder)
{
    folders.erase(&folder);
    folder.remMsg(this);
}

// save to one folder
void Message::save(Folder &folder)
{
    folders.insert(&folder);
    folder.addMsg(this);
}

// add this message to the folders that point to the parameter
void Message::add_to_folders(const Message &m)
{
    for ( Folder *f: m.folders )
        f->addMsg(this);
}

std::ostream &operator<<(std::ostream &os, const Message &m)
{
    os << "Current msg: " << m.contents << std::endl;
    os << "In [";
    for ( Folder *f: m.folders )
        os << f->getName() << ",";
    os << "]" << std::endl;
    return os;
}