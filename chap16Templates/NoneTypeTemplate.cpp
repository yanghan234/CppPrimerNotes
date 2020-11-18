#include <iostream>
using namespace std;

template <typename T>
int compare(const T &a, const T &b)
{
    if ( a < b ) return -1;
    if ( b < a ) return 1;
    return 0;
}

template <typename T, unsigned N>
int compare(const T (&a)[N], const T (&b)[N])
{
    for ( unsigned i = 0; i < N; ++i )
    {
        int comp = compare(a[i],b[i]);
        if ( comp ) return comp;
    }
    return 0;
}

int main(int argc, char const *argv[])
{
    const int N = 2;
    int a[N] = {1,2};
    int b[N] = {2,3};
    cout << compare(a,b) << endl;
    return 0;
}
