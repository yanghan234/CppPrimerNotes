#include <iostream>
using namespace std;

class Base
{
    public:
        Base()=default;
        virtual ~Base() = default;
};

class Derived: public Base
{
    public:
        Derived() { ptr = new int[100000]; }
        ~Derived() { delete[] ptr; } // override destructor
    private:
        int *ptr;
};

int main(int argc, char const *argv[])
{
    while ( true )
    {
        Base *ptr = new Derived();
        delete ptr;
    }
    return 0;
}
