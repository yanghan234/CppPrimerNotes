#include "folder.h"

void Folder::addMsg(const Message *m)
{
    messages.insert(m);
}

void Folder::remMsg(const Message *m)
{
    messages.erase(m);
}