#include <iostream>
#include <vector>
using namespace std;

class A
{
    friend bool operator<(const A &l, const A &r);
    private:
        int _a;
    public: 
        A()=default;
        A(int a) : _a(a) {}
};

bool operator<(const A &l, const A &r)
{
    return l._a < r._a;
}

int main()
{
    A a1(1), a2(2);
    cout << ( a1 < a2 ) << endl;
    vector<A> v1(10,2), v2(5,2);
    cout << ( v1 < v2 ) << endl;
}