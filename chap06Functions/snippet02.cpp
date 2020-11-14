#include <iostream>
using namespace std;

class Book
{
    public:
        Book()=default;
        Book(const Book& that)
        {
            cout << "Copy constructor is called." << endl;
        }
        Book operator=(const Book& that)
        {
            cout << "Assignment constructor is called." << endl;
        }
        ~Book()
        {
            cout << "Destructor is called." << endl;
        }
};

void func_by_value(const Book b)
{
}

void func_by_reference(const Book &b)
{
}

void func_by_pointer(const Book *b)
{
}

int main()
{
    Book b1;
    // call by value
    cout << "Call by value:" << endl;
    func_by_value(b1);
    cout << endl;

    // call by reference
    cout << "Call by reference:" << endl;
    func_by_reference(b1);
    cout << endl;

    // call by pointer
    cout << "Call by pointer:" << endl;
    func_by_pointer(&b1);
    cout << endl;

    cout << "Destructor of b1 should be called:" << endl;
    return 0;
}