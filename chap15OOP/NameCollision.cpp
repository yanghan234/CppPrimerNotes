#include <iostream>
using namespace std;
class Base
{
    public:
        string me = "base";
};

class Derived: public Base
{
    public:
        string me = "derived"; // hides me in base;
        string describe() { return me; }
        string describe2() { return Base::me; }
};

int main(int argc, char const *argv[])
{
    Derived dobj;
    cout << dobj.describe() << endl;
    cout << dobj.describe2() << endl;
    return 0;
}
