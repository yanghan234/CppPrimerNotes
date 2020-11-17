#include <iostream>
#include <vector>
#include <memory>
using namespace std;

class Base
{
    public:
        Base() = default;
        virtual string describe() const
        {
            return "Base";
        }
};

class Derived: public Base
{
    public:
        Derived() = default;
        string describe() const override
        {
            return "Derived";
        }
};

int main(int argc, char const *argv[])
{
    // container stores objects
    vector<Base> objvec;
    Base bobj;
    Derived dobj;
    objvec.push_back(bobj);
    objvec.push_back(dobj); // sliced down
    for ( auto &obj: objvec )
        cout << obj.describe() << endl;

    // container stores smart vectors
    vector<shared_ptr<Base>> vec;
    vec.push_back(shared_ptr<Base>(new Base()));
    vec.push_back(shared_ptr<Base>(new Derived())); // ok
    for ( auto &sp: vec )
        cout << sp->describe() << endl;
    
    return 0;
}
