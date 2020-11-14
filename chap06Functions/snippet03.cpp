#include <iostream>
#include <algorithm>
using namespace std;

void print(int (&arr)[5])
{
    for ( const auto &i: arr )
        cout << i << " ";
    cout << endl;
}

void print(int (*matrix)[10]){}

int main( )
{
    int arr1[5], arr2[10];
    fill(begin(arr1),end(arr1),0);
    fill(begin(arr2),end(arr2),0);
    print(arr1);
    // print(arr2); // illegal

    int arr3[2][10];
    int arr4[2][5];
    print(arr3);
    // print(arr4);  // illegal
}
