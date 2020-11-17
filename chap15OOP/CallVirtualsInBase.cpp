/*
 * This script checks what happens
 * when a base class calls virtual functions in its constructor.
 * When the base constructor executes, the derived objects do not exist,
 * so base constructors can only call virtuals in itself.
 * 
 */ 
#include <iostream>
using namespace std;

class Base
{
    public:
        Base() { func(); };
        virtual void func()
        {
            cout << "Base version of func" << endl;
        }
        virtual ~Base()
        {
            cout << "Base version of destructor" << endl;
        }
};

class Deriv: public Base
{
    public:
        Deriv(): Base() { func(); }
        void func() override
        {
            cout << "Deriv version of func" << endl;
        }
        ~Deriv()
        {
            cout << "Deriv version of destructor" << endl;
        }
};

int main()
{
    Base *b = new Deriv();
    b->func();
    delete b;
}