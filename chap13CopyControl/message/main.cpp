#include <iostream>
#include "message.h"
#include "folder.h"
using namespace std;
int main()
{
    Folder f1("f1"), f2("f2"), f3("f3");
    Message msg1("123");
    msg1.save(f1);
    msg1.save(f2);
    msg1.save(f3);
    cout << msg1;
    Message msg2 = msg1; 
    cout << msg2;
    return 0;
}