#include <iostream>
using namespace std;

class Book
{
    public:
        Book() { cout << this << endl; }
        ~Book(){};
};

int main()
{
    Book b;
    cout << &b << endl;

    cout << sizeof(b) << endl;
    return 0;
}