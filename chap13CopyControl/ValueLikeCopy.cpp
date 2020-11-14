/* 
 * Implement value-like copy for a class
 */
#include <iostream>
using namespace std;

class Foo
{
    friend ostream &operator<<(ostream &, const Foo &);
    private:
        string *ps; // We include a pointer on purpose
        int val;
    public:
        Foo(string str="",int v = 0) : ps(new string(str)), val(v) {
            cout << "Calling default constructor" << endl;
        }
        Foo(const Foo &);
        Foo &operator=(const Foo &);
        ~Foo();
};

Foo::Foo(const Foo &f)
{
    cout << "Calling copy constructor" << endl;
    this->ps = new string(*f.ps);
    this->val = f.val;
}

Foo &Foo::operator=(const Foo &f)
{
    cout << "Calling copy assignment constructor" << endl;
    if ( this != &f )
    {
        delete ps;
        ps = new string(*f.ps);
        val = f.val;
    }
    return *this;
}

Foo::~Foo()
{
    cout << "Calling detructor" << endl;
    delete ps;
}

ostream &operator<<(ostream &os, const Foo &f)
{
    os << "string: " << *f.ps << ", val: " << f.val;
    return os;
}

int main()
{
    Foo f1, f2("2",2), f3("123",345);
    cout << f1 << endl;
    cout << f2 << endl;
    cout << f3 << endl;
    f1 = f2;
    f2 = f3;
    cout << f1 << endl;
    cout << f2 << endl;
    cout << f3 << endl;
}
