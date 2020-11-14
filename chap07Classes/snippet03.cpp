#include <iostream>
using namespace std;

class Type
{
    friend ostream &operator<<(ostream &os, const Type &me);
    public:
        Type(int i, int j) : _i(i), _j(j) {}
        Type(int i) : Type(i,0) {}
    private:
        int _i;
        int _j;
};

ostream &operator<<(ostream &os, const Type &me)
{
    cout << "( i = " << me._i << ", j = " << me._j << ")";
    return os;
}

Type func()
{
    return 1;
}


int main()
{
    Type t = func();
    cout << t << endl;
}