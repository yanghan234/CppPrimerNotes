#include <iostream>
using namespace std;

class Type
{
    friend ostream& operator<<(ostream &, const Type &);
    public:
        Type &operator+=(const Type &that)
        {
            this->val += that.val;
            return *this;
        }
        Type()=default;
        Type(int _v) : val(_v){};
        Type(const Type& that) : val(that.val){};
        int getVal(){return val;}
    private:
        int val;
};

ostream& operator<<(ostream &os, const Type &me )
{
    os << me.val;
    return os;
}

int main()
{
    Type t1(1),t2(2);
    t1 += t2;
    cout << t1 << endl;
    return 0;
}