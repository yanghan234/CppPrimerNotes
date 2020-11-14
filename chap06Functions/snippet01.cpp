#include <iostream>
using namespace std;
void func()
{
    static int i;
    cout << i << endl;
    i++;
}

int main()
{
    for ( int i = 0; i < 10; i++ )
        func();
    return 0;
}