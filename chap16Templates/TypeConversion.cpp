#include <iostream>
using namespace std;

// two arguments the same type
template <typename T>
int compare_v1(const T &a, const T &b )
{
    if ( a < b ) return -1;
    if ( b < a ) return  1;
    return 0;
}

// two arguments can be of different types
template <typename A, typename B>
int compare_v2(const A &a, const B &b)
{
    if ( a < b ) return -1;
    if ( b < a ) return  1;
    return 0;
}

// normal conversion applies to second parameter
template <typename T>
int compare_v3(const T &a, const long &b)
{
    if ( a < b ) return -1;
    if ( b < a ) return  1;
    return 0;
}


int main(int argc, char const *argv[])
{
    long lng = 10;

    // two arguments of the same type
    // compare_v1(lng,1024); // error
    compare_v1(lng,1024L); // ok

    compare_v1<int>(lng,1024);
    compare_v1<long>(lng,1024);

    // two arguments can be of different types
    compare_v2(lng,1024); // ok
    compare_v2(lng,1024L); // ok

    // normal conversion applies to second argument
    compare_v3(lng,1024);
    compare_v3(lng,1024L);
    return 0;
}
