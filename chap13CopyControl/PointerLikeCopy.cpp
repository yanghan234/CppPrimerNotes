#include <iostream>
using namespace std;

class Foo
{
    friend ostream &operator<<(ostream &, const Foo &);
    private:
        string *ps;
        int val;
        int *counter;
    public:
        Foo(string str="", int _val = 0) : ps(new string(str)), val(_val), counter(new int(1))
        {
            cout << "Calling default constructor" << endl;
        }
        Foo(const Foo&);
        Foo &operator=(const Foo&);
        ~Foo();
};

Foo::Foo(const Foo &f)
{
    cout << "Calling copy constructor" << endl;
    ps = f.ps;
    val = f.val;
    counter = f.counter;
    ++*counter;
}

Foo &Foo::operator=(const Foo &f)
{
    cout << "Calling copy assignment" << endl;
    if ( this != &f)
    {
        if ( --*counter == 0 )
        {
            delete ps;
            delete counter;
        }
        ps = f.ps;
        val = f.val;
        counter = f.counter;
        ++*counter;
    }
    return *this;
}

Foo::~Foo()
{
    cout << "Calling destructor" << endl;
    if ( --*counter == 0 )
    {
        delete ps;
        delete counter;
    }
}

ostream &operator<<(ostream &os, const Foo &f)
{
    cout << "Counter: " << *f.counter << ", "
         << "String: " << *f.ps << ", "
         << "Value: " << f.val;
    return os;
}

int main()
{
    Foo f1, f2 = f1, f3, f4 = f3;
    cout << f1 << endl;
    cout << f2 << endl;
    cout << f3 << endl;
    cout << f4 << endl;
    f3 = f2;
    cout << f1 << endl;
    cout << f2 << endl;
    cout << f3 << endl;
    cout << f4 << endl;
    f4 = f3;
    cout << f1 << endl;
    cout << f2 << endl;
    cout << f3 << endl;
    cout << f4 << endl;
}