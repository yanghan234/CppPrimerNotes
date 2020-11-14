#include <iostream>
#include <vector>
using namespace std;

class A
{
    private:
        int _a;
        string _s;
    public:
        A(int a) : _a(a), _s(""){}
        A(string s) : _a(0), _s(s){}
};
int main()
{
    A a1(1);
    A a2("12");
    vector<A> v1;              // ok, empty container
    // vector<A> v2(10);       // error, class A has no default constructor
    vector<A> v3(10,1);        // ok, using implicit class type conversion
    // vector<A> v4(10,"123"); // error, because "123" -> string("123") -> A(string("123"))
                               // requires two-step conversion
    vector<A> v5(10,string("123")); // ok, using implicit class type conversion
    vector<A> v6(10,A(12));         // ok, explicitly call constructor
    vector<A> v7(10,A("123"));      // ok, explicitly call constructor
}