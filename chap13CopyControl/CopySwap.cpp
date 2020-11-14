#include <iostream>
using namespace std;

class Foo
{
    friend ostream &operator<<(ostream &,const Foo&);
    friend void swap(Foo &lhs, Foo &rhs);
    private:
        string *ps;
        int val;
    public:
        Foo(string str = "", int _val = 0) : ps(new string(str)), val(_val) {
            cout << "Calling default constructor" << endl;
        }
        Foo(const Foo&);
        Foo &operator=(Foo);
        ~Foo();
};

// value-like copy
Foo::Foo(const Foo &f)
{
    cout << "Calling copy constructor" << endl;
    ps = new string(*f.ps);
    val = f.val;
}

void swap(Foo &lhs, Foo &rhs)
{
    std::swap(lhs.ps,rhs.ps);
    std::swap(lhs.val,rhs.val);
}

// copy and swap assignment 
Foo &Foo::operator=(Foo f)
{
    // Note that f is passed by value,
    // after swap the original f out of this function is not changed.
    // When this function is called,
    // the copy constructor is firstly called
    // because f is passed by value.
    cout << "Calling copy assignment" << endl;
    swap(*this,f);
    return *this;
}

Foo::~Foo()
{
    cout << "Calling delete" << endl;
    delete ps;
}

ostream &operator<<(ostream &os, const Foo &f)
{
    cout << "String : " << *f.ps << ", "
         << "Value : " << f.val;
    return os;
}

int main()
{
    Foo f1("f1"), f2("f2"), f3("f3");
    cout << f1 << endl;
    cout << f2 << endl;
    cout << f3 << endl;
    f2 = f1;
    cout << f1 << endl;
    cout << f2 << endl;
    cout << f3 << endl;
    f3 = f1;
    cout << f1 << endl;
    cout << f2 << endl;
    cout << f3 << endl;
    return 0;
}
