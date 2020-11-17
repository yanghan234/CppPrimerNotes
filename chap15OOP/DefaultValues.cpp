#include <iostream>

using namespace std;

class Base
{
    public:
        virtual void func(int i = 0)
        {
            cout << "Base::func(), i = " << i << endl;
        }
};

class Derived: public Base
{
    public:
        void func(int i = 2) override
        {
            cout << "Derived::func(), i = " << i << endl;
        }
};

int main()
{
    Derived dobj;
    Base *bptr = &dobj;
    Derived *dptr = &dobj;
    bptr->func();
    dptr->func();
}